#include "evaluation_strategy.h"

#include "spdlog/spdlog.h"

EvaluationStrategy *EvaluationStrategy::getStrategy(IPKBQuerier *pkb, QueryClause *query_clause) {
  switch (query_clause->getClauseType()) {
    case ClauseType::kSuchThat: {
      spdlog::debug("Creating SuchThatEvaluationStrategy");
      return new SuchThatStrategy(pkb, dynamic_cast<SuchThatClause *>(query_clause));
    }
    case ClauseType::kPattern: {
      spdlog::debug("Creating PatternEvaluationStrategy");
      return new PatternStrategy(pkb, dynamic_cast<PatternClause *>(query_clause));
    }
    case ClauseType::kWith: {
      spdlog::debug("Creating WithEvaluationStrategy");
      return new WithStrategy(pkb, dynamic_cast<WithClause *>(query_clause));
    }
    default: throw EvaluationStrategyError("Invalid query clause type");
  }
}

EntitySet EvaluationStrategy::getCandidates(QueryReference *declaration) {
  EntitySet candidates;
  switch (declaration->getRefType()) {
    case ReferenceType::kElem:// fallthrough
    case ReferenceType::kSynonym: {
      return declaration->getContext();
    }
    case ReferenceType::kWildcard: {
      return pkb_->getEntities(declaration->getEntityType());
    }
    case ReferenceType::kInteger:// fallthrough
    case ReferenceType::kIdent: {
      std::basic_string<char> value = declaration->getReferenceValue();
      return pkb_->getEntitiesByString(value);
    }
  }
  return candidates;
}

bool EvaluationStrategy::shouldIntersect(QueryReference *declaration) {
  return !declaration->getContext().empty();
}

EntitySet EvaluationStrategy::intersect(const EntitySet &first, const EntitySet &second) {
  EntitySet result;
  for (auto *entity : first) {
    if (second.find(entity) != second.end()) {
      result.insert(entity);
    }
  }
  return result;
}
EntitySet EvaluationStrategy::copy_if(const EntitySet &candidates, const std::function<bool(Entity *)> &pred) {
  EntitySet result;
  for (auto *entity : candidates) {
    if (pred(entity)) {
      result.insert(entity);
    }
  }
  return result;
}
std::string EvaluationStrategy::getAttribute(AttributeType attribute_type, Entity *entity) {
  auto entity_type = entity->GetType();
  switch (attribute_type) {
    case AttributeType::kProcName:
      if (entity_type == EntityType::kProcedure) {
        return entity->GetValue();
      } else if (entity_type == EntityType::kCallStmt) {
        auto *call_stmt = static_cast<CallStmtEntity *>(entity);
        return call_stmt->GetProcName();
      }
    case AttributeType::kVarName:
      if (entity_type == EntityType::kVariable || entity_type == EntityType::kReadStmt
          || entity_type == EntityType::kPrintStmt) {
        return entity->GetValue();
      }
    case AttributeType::kValue:
      if (entity_type == EntityType::kConstant) {
        return entity->GetValue();
      }
    case AttributeType::kStmtNo:
      if (entity_type == EntityType::kAssignStmt || entity_type == EntityType::kCallStmt
          || entity_type == EntityType::kWhileStmt || entity_type == EntityType::kIfStmt
          || entity_type == EntityType::kPrintStmt || entity_type == EntityType::kReadStmt) {
        return entity->GetValue();
      }
    case AttributeType::kNone:break;
  }
  throw EvaluationStrategyError("Invalid attribute type");
}
std::string EvaluationStrategy::unwrapEntity(QueryReference *query_reference, Entity *entity) {
  if (query_reference->getRefType() == ReferenceType::kElem) {
    auto *elem_ref = static_cast<ElemReference *>(query_reference);
    return getAttribute(elem_ref->getAttribute(), entity);
  } else {
    return entity->GetValue();
  }
}

SubqueryResult SuchThatStrategy::evaluate() {
  RsType rs_type = this->clause_->getSuchThatType();
  spdlog::debug("Evaluating SuchThat clause with relationship {}", RsTypeToString(rs_type));

  QueryReference *first_param = this->clause_->getFirst();
  QueryReference *second_param = this->clause_->getSecond();
  EntitySet first_param_context = first_param->getContext();
  EntitySet second_param_context = second_param->getContext();
  EntitySet first_param_candidates = this->getCandidates(first_param);
  EntitySet second_param_candidates = this->getCandidates(second_param);

  // Evaluate the first parameter first
  if (first_param_context.size() <= second_param_context.size()) {
    auto second_matches = this->evaluateParameter(first_param, rs_type, false, second_param_candidates);
  } else {
    auto first_matches = this->evaluateParameter(second_param, rs_type, true, first_param_candidates);
  }
  auto matches = this->evaluateParameter(first_param, rs_type, false, second_param_candidates);
  return SubqueryResult(matches, first_param, second_param);
}

EntityPointerUnorderedMap SuchThatStrategy::evaluateParameter(QueryReference *param,
                                                              RsType rs_type,
                                                              bool invert_search,
                                                              const EntitySet &potential_matches) {
  spdlog::debug("Evaluating SuchThat parameter {} for {}, inverse = {}",
                param->getReferenceValue(),
                RsTypeToString(rs_type),
                invert_search);
  EntitySet candidates = this->getCandidates(param);
  std::string candidate_string;
  for (auto *candidate : candidates) {
    candidate_string += candidate->ToString() + ", ";
  }
  spdlog::debug("Candidates[{}]: {}", candidates.size(), candidate_string);
  EntityPointerUnorderedMap results;
  for (auto *entity : candidates) {
    EntitySet valid_entities = this->pkb_->getByRelationship(rs_type, entity, invert_search);
    auto intersected = EvaluationStrategy::intersect(valid_entities, potential_matches);
    results[entity] = intersected;
  }
  return results;
}

SubqueryResult PatternStrategy::evaluate() {
  spdlog::debug("Evaluating Pattern clause");

  SynonymReference *stmt_param = this->clause_->getSynonymDeclaration();
  QueryReference *var_param = this->clause_->getEntRef();
  ExpressionSpec *expr_param = this->clause_->getExpression();
  EntitySet stmt_param_context = stmt_param->getContext();

  auto stmt_matches = this->evaluateParameter(var_param, expr_param, stmt_param_context);
  return SubqueryResult(stmt_matches, var_param, stmt_param);
}

EntityPointerUnorderedMap PatternStrategy::evaluateParameter(QueryReference *var_param,
                                                             ExpressionSpec *expr_param,
                                                             const EntitySet &potential_matches) {
  spdlog::debug("Evaluating Pattern parameter {} for {}", var_param->getReferenceValue(), expr_param->toString());
  EntitySet candidates = this->getCandidates(var_param);
  std::string candidate_string;
  for (auto *candidate : candidates) {
    candidate_string += candidate->ToString() + ", ";
  }
  spdlog::debug("Candidates[{}]: {}", candidates.size(), candidate_string);
  EntityPointerUnorderedMap results;
  std::string expr = expr_param->toString();
  for (auto *entity : candidates) {
    EntitySet valid_entities = this->pkb_->getByPattern(entity, expr, expr_param->isWild());
    auto intersected = EvaluationStrategy::intersect(valid_entities, potential_matches);
    results[entity] = intersected;
  }
  return results;
}

SubqueryResult WithStrategy::evaluate() {
  spdlog::debug("Evaluating With clause");

  Comparator comparator = this->clause_->getComparator();
  QueryReference *first_param = this->clause_->getFirst();
  QueryReference *second_param = this->clause_->getSecond();

  auto matches = this->evaluateParameter(first_param, second_param, comparator);
  return SubqueryResult(matches, first_param, second_param);
}
EntityPointerUnorderedMap WithStrategy::evaluateParameter(QueryReference *first,
                                                          QueryReference *second,
                                                          Comparator comparator) {
  spdlog::debug("Evaluating With parameter {} = {}", first->toString(), second->toString());
  EntitySet first_candidates = this->getCandidates(first);
  EntitySet second_candidates = this->getCandidates(second);
  std::string candidate_string;
  for (auto *candidate : first_candidates) {
    candidate_string += candidate->ToString() + ", ";
  }
  spdlog::debug("Candidates[{}]: {}", first_candidates.size(), candidate_string);

  EntityPointerUnorderedMap results;
  for (auto *entity : first_candidates) {
    auto target = EvaluationStrategy::unwrapEntity(first, entity);
    auto const pred = [&comparator, &second, &target](Entity *other) {
      switch (comparator) {
        case Comparator::kEquals: {
          return unwrapEntity(second, other) == target;
        }
      }
      return false;
    };
    auto valid_entities = EvaluationStrategy::copy_if(second_candidates, pred);
    results[entity] = valid_entities;
  }
  return results;
}
