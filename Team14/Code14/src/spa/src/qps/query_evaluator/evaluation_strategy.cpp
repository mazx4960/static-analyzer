#include "evaluation_strategy.h"

#include "spdlog/spdlog.h"

/**
 * Factory method to get the correct evaluation strategy for the given query clause.
 * @param pkb PKB interface.
 * @param query_clause query clause.
 * @return pointer to EvaluationStrategy instance.
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

/**
 * Get the candidates for the given query declaration.
 * @param query_declaration query declaration.
 * @return set of Entity candidates.
 */
EntityPointerUnorderedSet EvaluationStrategy::getCandidates(QueryDeclaration *declaration) {
  EntityPointerUnorderedSet candidates;
  if (declaration->getType() == EntityType::kString || declaration->getType() == EntityType::kInteger) {
    std::basic_string<char> value = declaration->toString();
    candidates = pkb_->getEntitiesByString(value);
  } else if (declaration->getType() == EntityType::kWildcardStmt) {
    EntityPointerUnorderedSet all_variables = pkb_->getEntities(EntityType::kStatement);
    candidates = all_variables;
  } else if (declaration->getType() == EntityType::kWildcardEnt) {
    EntityPointerUnorderedSet all_variables = pkb_->getEntities(EntityType::kVariable);
    candidates = all_variables;
  } else {
    candidates = declaration->getContext();
  }
  return candidates;
}

/**
 * Checks if the context of two QueryDeclarations should be intersected.
 * If the context is empty it means that the QueryDeclaration is a wildcard or a literal.
 * @param declaration query declaration.
 * @return true if the context should be intersected, false otherwise.
 */
bool EvaluationStrategy::shouldIntersect(QueryDeclaration *declaration) { return !declaration->getContext().empty(); }

/**
 * Intersect the context of two QueryDeclarations.
 * @param first first set of Entity pointers.
 * @param second second set of Entity pointers.
 * @return intersection of sets of Entity pointers.
 */
EntityPointerUnorderedSet EvaluationStrategy::intersect(const EntityPointerUnorderedSet &first,
                                                        const EntityPointerUnorderedSet &second) {
  EntityPointerUnorderedSet result;
  for (auto *entity : first) {
    if (second.find(entity) != second.end()) { result.insert(entity); }
  }
  return result;
}

/**
 * Evaluate the SuchThat clause.
 * @return true if query clause has results, false otherwise.
 */
SubqueryResult SuchThatStrategy::evaluate() {
  RsType rs_type = this->clause_->getSuchThatType();
  spdlog::debug("Evaluating SuchThat clause with relationship {}", RsTypeToString(rs_type));

  QueryDeclaration *first_param = this->clause_->getFirst();
  QueryDeclaration *second_param = this->clause_->getSecond();
  EntityPointerUnorderedSet first_param_context = first_param->getContext();
  EntityPointerUnorderedSet second_param_context = second_param->getContext();
  EntityPointerUnorderedSet first_param_candidates = this->getCandidates(first_param);
  EntityPointerUnorderedSet second_param_candidates = this->getCandidates(second_param);

  bool has_results;

  // Evaluate the first parameter first
  if (first_param_context.size() <= second_param_context.size()) {
    auto second_matches = this->evaluateParameter(first_param, rs_type, false, second_param_candidates);
    //if (this->shouldIntersect(second_param)) { second_param->setContext(second_matches); }
    has_results = !second_matches.empty();
  } else {
    auto first_matches = this->evaluateParameter(second_param, rs_type, true, first_param_candidates);
    //if (this->shouldIntersect(first_param)) { first_param->setContext(first_matches); }
    has_results = !first_matches.empty();
  }

  auto matches = this->evaluateParameter(first_param, rs_type, false, second_param_candidates);
  return SubqueryResult(matches, first_param, second_param);
}

/**
 * Evaluate query parameter.
 * @param param parameter to evaluated.
 * @param rs_type Relationship type.
 * @param invert_search true if searching by second parameter (e.g. searching by s2 in Follows(s1, s2)).
 * @return set of Entity pointers matching the parameter and relationship type.
 */
EntityPointerUnorderedMap SuchThatStrategy::evaluateParameter(QueryDeclaration *param, RsType rs_type,
                                                              bool invert_search,
                                                              const EntityPointerUnorderedSet &potential_matches) {
  spdlog::debug("Evaluating SuchThat parameter {} for {}, inverse = {}", param->toString(), RsTypeToString(rs_type),
                invert_search);
  EntityPointerUnorderedSet candidates = this->getCandidates(param);
  std::string candidate_string;
  for (auto *candidate : candidates) { candidate_string += candidate->ToString() + ", "; }
  spdlog::debug("Candidates[{}]: {}", candidates.size(), candidate_string);
  EntityPointerUnorderedMap results;
  for (auto *entity : candidates) {
    EntityPointerUnorderedSet valid_entities = this->pkb_->getByRelationship(rs_type, entity, invert_search);
    auto intersected = EvaluationStrategy::intersect(valid_entities, potential_matches);
    results[entity] = intersected;
  }
  return results;
}

/**
 * Evaluate the Pattern clause.
 * @return true if query clause has results, false otherwise.
 */
SubqueryResult PatternStrategy::evaluate() {
  spdlog::debug("Evaluating Pattern clause");

  QueryDeclaration *stmt_param = this->clause_->getFirst();
  QueryDeclaration *var_param = this->clause_->getSecond();
  QueryDeclaration *expr_param = this->clause_->getThird();
  EntityPointerUnorderedSet stmt_param_context = stmt_param->getContext();

  auto stmt_matches = this->evaluateParameter(var_param, expr_param, stmt_param_context);
  return SubqueryResult(stmt_matches, var_param, stmt_param);
}

/**
 * Evaluate pattern clause given parameters.
 * @param var_param Left-hand side parameter.
 * @param expr_param Right-hand side expression.
 * @return set of Entity pointers matching the parameter and expression.
 */
EntityPointerUnorderedMap PatternStrategy::evaluateParameter(QueryDeclaration *var_param, QueryDeclaration *expr_param,
                                                             const EntityPointerUnorderedSet &potential_matches) {
  spdlog::debug("Evaluating Pattern parameter {} for {}", var_param->toString(), expr_param->toString());
  EntityPointerUnorderedSet candidates = this->getCandidates(var_param);
  std::string candidate_string;
  for (auto *candidate : candidates) { candidate_string += candidate->ToString() + ", "; }
  spdlog::debug("Candidates[{}]: {}", candidates.size(), candidate_string);
  EntityPointerUnorderedMap results;
  std::string expr = expr_param->toString();
  for (auto *entity : candidates) {
    EntityPointerUnorderedSet valid_entities = this->pkb_->getByPattern(entity, expr);
    auto intersected = EvaluationStrategy::intersect(valid_entities, potential_matches);
    results[entity] = intersected;
  }
  return results;
}
