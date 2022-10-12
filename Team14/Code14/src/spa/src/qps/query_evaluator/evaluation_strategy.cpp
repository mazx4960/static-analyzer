#include "evaluation_strategy.h"

#include "spdlog/spdlog.h"

EvaluationStrategy *EvaluationStrategy::getStrategy(IPKBQuerier *pkb, QueryClause *query_clause) {
  switch (query_clause->getClauseType()) {
    case ClauseType::kSuchThat: {
      spdlog::debug("Creating SuchThatEvaluationStrategy");
      return new SuchThatStrategy(pkb, static_cast<SuchThatClause *>(query_clause));
    }
    case ClauseType::kPattern: {
      spdlog::debug("Creating PatternEvaluationStrategy");
      return new PatternStrategy(pkb, static_cast<PatternClause *>(query_clause));
    }
    default: throw EvaluationStrategyCreationError("Invalid query clause type");
  }
}

EntityPointerUnorderedSet EvaluationStrategy::getCandidates(QueryReference *declaration) {
  EntityPointerUnorderedSet candidates;
  switch (declaration->getRefType()) {
    case ReferenceType::kSynonym: { return declaration->getContext(); }
    case ReferenceType::kWildcard: { return pkb_->getEntities(declaration->getEntityType()); }
    case ReferenceType::kInteger: // fallthrough
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

EntityPointerUnorderedSet EvaluationStrategy::intersect(const EntityPointerUnorderedSet &first, const EntityPointerUnorderedSet &second) {
  EntityPointerUnorderedSet result;
  for (auto *entity : first) {
    if (second.find(entity) != second.end()) {
      result.insert(entity);
    }
  }
  return result;
}

SubqueryResult SuchThatStrategy::evaluate() {
  RsType rs_type = this->clause_->getSuchThatType();
  spdlog::debug("Evaluating SuchThat clause with relationship {}", RsTypeToString(rs_type));

  QueryReference *first_param = this->clause_->getFirst();
  QueryReference *second_param = this->clause_->getSecond();
  EntityPointerUnorderedSet first_param_context = first_param->getContext();
  EntityPointerUnorderedSet second_param_context = second_param->getContext();
  EntityPointerUnorderedSet first_param_candidates = this->getCandidates(first_param);
  EntityPointerUnorderedSet second_param_candidates = this->getCandidates(second_param);

  // Evaluate the first parameter first
  if (first_param_context.size() <= second_param_context.size()) {
    auto second_matches = this->evaluateParameter(first_param, rs_type, false, second_param_candidates);
  } else {
    auto first_matches = this->evaluateParameter(second_param, rs_type, true, first_param_candidates);
  }
  auto matches = this->evaluateParameter(first_param, rs_type, false, second_param_candidates);
  return SubqueryResult(matches, first_param, second_param);
}

EntityPointerUnorderedMap SuchThatStrategy::evaluateParameter(QueryReference *param, RsType rs_type,
                                                              bool invert_search,
                                                              const EntityPointerUnorderedSet &potential_matches) {
  spdlog::debug("Evaluating SuchThat parameter {} for {}, inverse = {}",
                param->getReferenceValue(), RsTypeToString(rs_type), invert_search);
  EntityPointerUnorderedSet candidates = this->getCandidates(param);
  std::string candidate_string;
  for (auto *candidate : candidates) {
    candidate_string += candidate->ToString() + ", ";
  }
  spdlog::debug("Candidates[{}]: {}", candidates.size(), candidate_string);
  EntityPointerUnorderedMap results;
  for (auto *entity : candidates) {
    EntityPointerUnorderedSet valid_entities = this->pkb_->getByRelationship(rs_type, entity, invert_search);
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
  EntityPointerUnorderedSet stmt_param_context = stmt_param->getContext();

  auto stmt_matches = this->evaluateParameter(var_param, expr_param, stmt_param_context);
  return SubqueryResult(stmt_matches, var_param, stmt_param);
}

EntityPointerUnorderedMap PatternStrategy::evaluateParameter(QueryReference *var_param, ExpressionSpec *expr_param,
                                                             const EntityPointerUnorderedSet &potential_matches) {
  spdlog::debug("Evaluating Pattern parameter {} for {}", var_param->getReferenceValue(), expr_param->toString());
  EntityPointerUnorderedSet candidates = this->getCandidates(var_param);
  std::string candidate_string;
  for (auto *candidate : candidates) {
    candidate_string += candidate->ToString() + ", ";
  }
  spdlog::debug("Candidates[{}]: {}", candidates.size(), candidate_string);
  EntityPointerUnorderedMap results;
  std::string expr = expr_param->toString();
  for (auto *entity : candidates) {
    EntityPointerUnorderedSet valid_entities = this->pkb_->getByPattern(entity, expr, expr_param->isWild());
    auto intersected = EvaluationStrategy::intersect(valid_entities, potential_matches);
    results[entity] = intersected;
  }
  return results;
}
