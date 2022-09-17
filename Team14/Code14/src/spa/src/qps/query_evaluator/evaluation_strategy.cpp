#include "evaluation_strategy.h"

#include "spdlog/spdlog.h"

/**
 * Factory method
 */
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

EntityPointerUnorderedSet EvaluationStrategy::getCandidates(QueryDeclaration *declaration) {
  EntityPointerUnorderedSet candidates;
  if (declaration->getType() == EntityType::kString) {
    candidates.insert(new VariableEntity(declaration->toString()));
  } else if (declaration->getType() == EntityType::kInteger) {// TODO: get entity from pkb
    candidates.insert(new StmtEntity(declaration->toString()));
  } else if (declaration->getType() == EntityType::kWildcard) {// TODO: change to wild stmt and wild variable
    EntityPointerUnorderedSet all_variables = pkb_->getEntities(EntityType::kVariable);
    for (auto *entity : all_variables) { candidates.insert(entity); }
  } else {
    candidates = declaration->getContext();
  }
  return candidates;
}
bool EvaluationStrategy::shouldIntersect(QueryDeclaration *declaration) {
  return declaration->getType() != EntityType::kString && declaration->getType() != EntityType::kInteger
      && declaration->getType() != EntityType::kWildcard;
}
EntityPointerUnorderedSet EvaluationStrategy::intersectContext(const EntityPointerUnorderedSet &first,
                                                               EntityPointerUnorderedSet second) {
  EntityPointerUnorderedSet result;
  for (auto *entity : first) {
    if (second.find(entity) != second.end()) { result.insert(entity); }
  }
  return result;
}

void SuchThatStrategy::evaluate() {
  RsType rs_type = this->clause_->getSuchThatType();
  spdlog::debug("Evaluating SuchThat clause with relationship {}", RsTypeToString(rs_type));

  QueryDeclaration *first_param = this->clause_->getFirst();
  QueryDeclaration *second_param = this->clause_->getSecond();
  EntityPointerUnorderedSet first_param_context = first_param->getContext();
  EntityPointerUnorderedSet second_param_context = second_param->getContext();

  // Evaluate the first parameter first
  if (first_param_context.size() <= second_param_context.size()) {
    auto second_matches = this->evaluateParameter(first_param, rs_type, false);
    if (this->shouldIntersect(second_param)) {
      auto intersected = EvaluationStrategy::intersectContext(second_matches, second_param_context);
      second_param->setContext(intersected);
    }
    auto first_matches = this->evaluateParameter(second_param, rs_type, true);
    if (this->shouldIntersect(first_param)) {
      auto intersected = EvaluationStrategy::intersectContext(first_matches, first_param_context);
      first_param->setContext(intersected);
    }
  } else {
    auto first_matches = this->evaluateParameter(second_param, rs_type, true);
    if (this->shouldIntersect(first_param)) {
      auto intersected = EvaluationStrategy::intersectContext(first_matches, first_param_context);
      first_param->setContext(intersected);
    }
    auto second_matches = this->evaluateParameter(first_param, rs_type, false);
    if (this->shouldIntersect(second_param)) {
      auto intersected = EvaluationStrategy::intersectContext(second_matches, second_param_context);
      second_param->setContext(intersected);
    }
  }
}
EntityPointerUnorderedSet SuchThatStrategy::evaluateParameter(QueryDeclaration *param, RsType rs_type,
                                                              bool invert_search) {
  spdlog::debug("Evaluating SuchThat parameter {} for {}, inverse = {}", param->toString(), RsTypeToString(rs_type),
                invert_search);
  EntityPointerUnorderedSet candidates = this->getCandidates(param);
  std::string candidate_string;
  for (auto *candidate : candidates) { candidate_string += candidate->ToString() + ", "; }
  spdlog::debug("Candidates: {}", candidate_string);
  EntityPointerUnorderedSet results;
  for (auto *entity : candidates) {
    EntityPointerUnorderedSet valid_entities = this->pkb_->getByRelationship(rs_type, entity, invert_search);
    for (auto *valid_entity : valid_entities) { results.insert(valid_entity); }
  }
  spdlog::debug("Results: {}", results.size());
  return results;
}

void PatternStrategy::evaluate() {
  spdlog::debug("Evaluating Pattern clause");

  QueryDeclaration *stmt_param = this->clause_->getFirst();
  QueryDeclaration *var_param = this->clause_->getSecond();
  QueryDeclaration *expr_param = this->clause_->getThird();

  auto stmt_matches = this->evaluateParameter(var_param, expr_param);
  auto intersected = EvaluationStrategy::intersectContext(stmt_matches, stmt_param->getContext());
  stmt_param->setContext(intersected);
}
EntityPointerUnorderedSet PatternStrategy::evaluateParameter(QueryDeclaration *var_param,
                                                             QueryDeclaration *expr_param) {
  spdlog::debug("Evaluating Pattern parameter {} for {}", var_param->toString(), expr_param->toString());
  EntityPointerUnorderedSet candidates = this->getCandidates(var_param);
  std::string candidate_string;
  for (auto *candidate : candidates) { candidate_string += candidate->ToString() + ", "; }
  spdlog::debug("Candidates: {}", candidate_string);
  EntityPointerUnorderedSet results;
  std::string expr = expr_param->toString();
  for (auto *entity : candidates) {
    EntityPointerUnorderedSet valid_entities = this->pkb_->getByPattern(entity, expr);
    if (valid_entities.empty()) {
      var_param->removeEntityFromContext(entity);
    }
    results.merge(valid_entities);
  }
  spdlog::debug("Results: {}", results.size());
  return results;
}
