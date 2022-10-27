#include "evaluation_strategy.h"

#include "spdlog/spdlog.h"

EvaluationStrategy *EvaluationStrategy::getStrategy(IPKBQuerier *pkb, QueryClause *query_clause) {
  spdlog::info("Creating evaluation strategy for {}", query_clause->toString());
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
    default:
      throw EvaluationStrategyError("Invalid query clause type");
  }
}

EntitySet EvaluationStrategy::getCandidates(Context *ctx, QueryReference *declaration) {
  EntitySet candidates;
  std::basic_string<char> value;
  switch (declaration->getRefType()) {
    case ReferenceType::kAttr:
      return ctx->Get(static_cast<AttrReference *>(declaration)->getSynonym());
    case ReferenceType::kSynonym: {
      return ctx->Get(static_cast<SynonymReference *>(declaration)->getSynonym());
    }
    case ReferenceType::kWildcard: {
      return pkb_->getEntities(static_cast<WildcardReference *>(declaration)->getEntityType());
    }
    case ReferenceType::kInteger:
      value = static_cast<IdentReference *>(declaration)->getValue();
      return pkb_->getEntitiesByString(value);
    case ReferenceType::kIdent: {
      value = static_cast<IntegerReference *>(declaration)->getValue();
      return pkb_->getEntitiesByString(value);
    }
  }
  return candidates;
}

std::string EvaluationStrategy::unwrapEntity(QueryReference *query_reference, Entity *entity) {
  if (query_reference->getRefType() == ReferenceType::kAttr) {
    auto *attr_ref = static_cast<AttrReference *>(query_reference);
    return attr_ref->getAttribute(entity);
  }
  return entity->GetValue();
}
void EvaluationStrategy::updateContext(Context *ctx, QueryReference *synonym_ref, EntitySet &entities) {
  if (synonym_ref->getRefType() != ReferenceType::kAttr && synonym_ref->getRefType() != ReferenceType::kSynonym) {
    return;
  }
  auto *synonym = static_cast<ElemReference *>(synonym_ref)->getSynonym();
  ctx->Set(synonym, entities);
}

SubqueryResult SuchThatStrategy::evaluate(Context *ctx) {
  RsType rs_type = this->clause_->getSuchThatType();
  QueryReference *first_param = this->clause_->getFirst();
  QueryReference *second_param = this->clause_->getSecond();
  spdlog::debug("Evaluating SuchThat clause {}: {}, {}",
                RsTypeToString(rs_type),
                first_param->toString(),
                second_param->toString());

  auto matches = this->evaluateParameter(ctx, rs_type, first_param, second_param);
  return SubqueryResult(matches, first_param, second_param);
}

EntityPointerUnorderedMap SuchThatStrategy::evaluateParameter(Context *ctx,
                                                              RsType rs_type,
                                                              QueryReference *first,
                                                              QueryReference *second) {
  EntitySet candidates = this->getCandidates(ctx, first);
  EntitySet potential_matches = this->getCandidates(ctx, second);
  std::string candidate_string;
  for (auto *candidate : candidates) {
    candidate_string += candidate->ToString() + ", ";
  }
  spdlog::debug("Candidates[{}]: {}", candidates.size(), candidate_string);

  EntityPointerUnorderedMap results;
  EntitySet valid_first;
  EntitySet valid_second;
  for (auto *entity : candidates) {
    EntitySet valid_entities = this->pkb_->getByRelationship(rs_type, entity, false);
    auto intersected = intersect(valid_entities, potential_matches);
    if (intersected.empty()) {
      continue;
    }
    valid_first.insert(entity);
    valid_second.insert(intersected.begin(), intersected.end());
    results[entity] = intersected;
  }
  this->updateContext(ctx, first, valid_first);
  this->updateContext(ctx, second, valid_second);
  return results;
}

SubqueryResult PatternStrategy::evaluate(Context *ctx) {
  SynonymReference *stmt_param = this->clause_->getStmtRef();
  QueryReference *var_param = this->clause_->getEntRef();
  ExpressionSpec *expr_param = this->clause_->getFirstExpression();
  spdlog::debug("Evaluating Pattern clause {}: {}, {}",
                stmt_param->toString(),
                var_param->toString(),
                expr_param->toString());

  auto stmt_matches = this->evaluateParameter(ctx, stmt_param, var_param, expr_param);
  return SubqueryResult(stmt_matches, var_param, stmt_param);
}

EntityPointerUnorderedMap PatternStrategy::evaluateParameter(Context *ctx,
                                                             SynonymReference *stmt_param,
                                                             QueryReference *var_param,
                                                             ExpressionSpec *expr_param) {
  EntitySet candidates = this->getCandidates(ctx, var_param);
  EntitySet potential_matches = this->getCandidates(ctx, stmt_param);
  std::string candidate_string;
  for (auto *candidate : candidates) {
    candidate_string += candidate->ToString() + ", ";
  }
  spdlog::debug("Candidates[{}]: {}", candidates.size(), candidate_string);

  EntityPointerUnorderedMap results;
  EntitySet valid_stmt;
  EntitySet valid_var;
  std::string expr = expr_param->toString();
  for (auto *var : candidates) {
    EntitySet valid_entities = this->pkb_->getByPattern(var, expr, expr_param->isWild());
    auto intersected = intersect(valid_entities, potential_matches);
    if (intersected.empty()) {
      continue;
    }
    valid_var.insert(var);
    valid_stmt.insert(intersected.begin(), intersected.end());
    results[var] = intersected;
  }
  this->updateContext(ctx, var_param, valid_var);
  this->updateContext(ctx, stmt_param, valid_stmt);
  return results;
}

SubqueryResult WithStrategy::evaluate(Context *ctx) {
  Comparator comparator = this->clause_->getComparator();
  QueryReference *first_param = this->clause_->getFirst();
  QueryReference *second_param = this->clause_->getSecond();
  spdlog::debug("Evaluating With clause {} = {}", first_param->toString(), second_param->toString());

  auto matches = this->evaluateParameter(ctx, first_param, second_param, comparator);
  return SubqueryResult(matches, first_param, second_param);
}
EntityPointerUnorderedMap WithStrategy::evaluateParameter(Context *ctx,
                                                          QueryReference *first,
                                                          QueryReference *second,
                                                          Comparator comparator) {
  EntitySet first_candidates = this->getCandidates(ctx, first);
  EntitySet second_candidates = this->getCandidates(ctx, second);
  std::string candidate_string1;
  std::string candidate_string2;
  for (auto *candidate : first_candidates) {
    candidate_string1 += candidate->ToString() + ", ";
  }
  for (auto *candidate : second_candidates) {
    candidate_string2 += candidate->ToString() + ", ";
  }
  spdlog::debug("Candidates1[{}]: {}", first_candidates.size(), candidate_string1);
  spdlog::debug("Candidates2[{}]: {}", second_candidates.size(), candidate_string2);

  EntityPointerUnorderedMap results;
  EntitySet valid_first;
  EntitySet valid_second;
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
    auto valid_entities = copy_if(second_candidates, pred);
    if (valid_entities.empty()) {
      continue;
    }
    valid_first.insert(entity);
    valid_second.insert(valid_entities.begin(), valid_entities.end());
    results[entity] = valid_entities;
  }
  this->updateContext(ctx, first, valid_first);
  this->updateContext(ctx, second, valid_second);
  return results;
}
