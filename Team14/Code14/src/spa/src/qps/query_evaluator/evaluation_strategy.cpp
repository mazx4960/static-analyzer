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

SubqueryResult SuchThatStrategy::evaluate(Context *ctx) {
  RsType rs_type = this->clause_->getSuchThatType();
  spdlog::debug("Evaluating SuchThat clause with relationship {}", RsTypeToString(rs_type));

  QueryReference *first_param = this->clause_->getFirst();
  QueryReference *second_param = this->clause_->getSecond();
  EntitySet first_param_candidates = this->getCandidates(ctx, first_param);
  EntitySet second_param_candidates = this->getCandidates(ctx, second_param);

  auto matches = this->evaluateParameter(ctx, first_param, rs_type, false, second_param_candidates);
  return SubqueryResult(matches, first_param, second_param);
}

EntityPointerUnorderedMap SuchThatStrategy::evaluateParameter(Context *ctx,
                                                              QueryReference *param,
                                                              RsType rs_type,
                                                              bool invert_search,
                                                              const EntitySet &potential_matches) {
  spdlog::debug("Evaluating SuchThat parameter for {}, inverse = {}", RsTypeToString(rs_type), invert_search);
  EntitySet candidates = this->getCandidates(ctx, param);
  std::string candidate_string;
  for (auto *candidate : candidates) {
    candidate_string += candidate->ToString() + ", ";
  }
  spdlog::debug("Candidates[{}]: {}", candidates.size(), candidate_string);
  EntityPointerUnorderedMap results;
  for (auto *entity : candidates) {
    EntitySet valid_entities = this->pkb_->getByRelationship(rs_type, entity, invert_search);
    auto intersected = intersect(valid_entities, potential_matches);
    results[entity] = intersected;
  }
  return results;
}

SubqueryResult PatternStrategy::evaluate(Context *ctx) {
  spdlog::debug("Evaluating Pattern clause");

  SynonymReference *stmt_param = this->clause_->getStmtRef();
  QueryReference *var_param = this->clause_->getEntRef();
  ExpressionSpec *expr_param = this->clause_->getFirstExpression();
  EntitySet stmt_param_context = this->getCandidates(ctx, stmt_param);

  auto stmt_matches = this->evaluateParameter(ctx, var_param, expr_param, stmt_param_context);
  return SubqueryResult(stmt_matches, var_param, stmt_param);
}

EntityPointerUnorderedMap PatternStrategy::evaluateParameter(Context *ctx,
                                                             QueryReference *var_param,
                                                             ExpressionSpec *expr_param,
                                                             const EntitySet &potential_matches) {
  spdlog::debug("Evaluating Pattern parameter for {}", expr_param->toString());
  EntitySet candidates = this->getCandidates(ctx, var_param);
  std::string candidate_string;
  for (auto *candidate : candidates) {
    candidate_string += candidate->ToString() + ", ";
  }
  spdlog::debug("Candidates[{}]: {}", candidates.size(), candidate_string);
  EntityPointerUnorderedMap results;
  std::string expr = expr_param->toString();
  for (auto *entity : candidates) {
    EntitySet valid_entities = this->pkb_->getByPattern(entity, expr, expr_param->isWild());
    auto intersected = intersect(valid_entities, potential_matches);
    results[entity] = intersected;
  }
  return results;
}

SubqueryResult WithStrategy::evaluate(Context *ctx) {
  spdlog::debug("Evaluating With clause");

  Comparator comparator = this->clause_->getComparator();
  QueryReference *first_param = this->clause_->getFirst();
  QueryReference *second_param = this->clause_->getSecond();

  auto matches = this->evaluateParameter(ctx, first_param, second_param, comparator);
  return SubqueryResult(matches, first_param, second_param);
}
EntityPointerUnorderedMap WithStrategy::evaluateParameter(Context *ctx,
                                                          QueryReference *first,
                                                          QueryReference *second,
                                                          Comparator comparator) {
  spdlog::debug("Evaluating With parameter {} = {}", first->toString(), second->toString());
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
    results[entity] = valid_entities;
  }
  return results;
}
