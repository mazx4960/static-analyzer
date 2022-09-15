#include "evaluation_strategy.h"

/**
 * Factory method
 */
EvaluationStrategy *EvaluationStrategy::getStrategy(IPKBQuerier *pkb, QueryClause *query_clause) {
  switch (query_clause->getClauseType()) {
    case ClauseType::kSuchThat: return new SuchThatStrategy(pkb, static_cast<SuchThatClause *>(query_clause));
    case ClauseType::kPattern: return new PatternStrategy(pkb, static_cast<PatternClause *>(query_clause));
    default: throw EvaluationStrategyCreationError("Invalid query clause type");
  }
}

// TODO(howtoosee) implement SuchThat strategy
void SuchThatStrategy::evaluate() {
  RsType rs_type = this->clause_->getSuchThatType();

  QueryDeclaration *first_param = this->clause_->getFirst();
  QueryDeclaration *second_param = this->clause_->getSecond();
  std::unordered_set<Entity *, EntityHashFunction, EntityPointerEquality>
      first_param_context = first_param->getContext();
  std::unordered_set<Entity *, EntityHashFunction, EntityPointerEquality>
      second_param_context = second_param->getContext();

  if (first_param_context.size() <= second_param_context.size()) {
    // Intersect 2nd set first
    intersectContext(first_param, second_param, rs_type, false);
    // Then intersect 1st set
    intersectContext(second_param, first_param, rs_type, true);

  } else {
    // Intersect 1st set first
    intersectContext(second_param, first_param, rs_type, true);
    // Then intersect 2nd set
    intersectContext(first_param, second_param, rs_type, false);
  }
}

void SuchThatStrategy::intersectContext(QueryDeclaration *param_to_send, QueryDeclaration *param_to_be_intersected,
                                        RsType rs_type, bool invert_search) {
  std::unordered_set<Entity *, EntityHashFunction, EntityPointerEquality> valid_entities_for_other_param;

  for (auto *context_entity : param_to_send->getContext()) {
    std::unordered_set<Entity *, EntityHashFunction, EntityPointerEquality>
        valid_entities = this->pkb_->getByRelationship(rs_type, context_entity, invert_search);
    if (valid_entities.empty()) {
      param_to_send->getContext().erase(context_entity);
    } else {
      for (auto *entity : valid_entities) { valid_entities_for_other_param.insert(entity); }
    }
  }

  std::unordered_set<Entity *, EntityHashFunction, EntityPointerEquality> intersected_results;

  std::set_intersection(valid_entities_for_other_param.begin(), valid_entities_for_other_param.end(),
                        param_to_be_intersected->getContext().begin(), param_to_be_intersected->getContext().end(),
                        std::inserter(intersected_results, intersected_results.begin()));

  param_to_be_intersected->setContext(intersected_results);
}

void PatternStrategy::evaluate() {
  QueryDeclaration *first_param = this->clause_->getFirst();
  QueryDeclaration *second_param = this->clause_->getSecond();
  QueryDeclaration *third_param = this->clause_->getThird();

  intersectContext(first_param, second_param, third_param);
}
void PatternStrategy::intersectContext(QueryDeclaration *assign_param, QueryDeclaration *left_param,
                                       QueryDeclaration *right_param) {
  std::string pattern_substring = right_param->getString();
  std::unordered_set<Entity *, EntityHashFunction, EntityPointerEquality> assign_result;
  std::unordered_set<Entity *, EntityHashFunction, EntityPointerEquality> intersected_results;

  if (left_param->getType() == EntityType::kWildcard) {
    std::unordered_set<Entity *, EntityHashFunction, EntityPointerEquality> all_variables = pkb_->getEntities(EntityType::kVariable);
    for (auto *variable_entity : all_variables) {
      assign_result.merge(pkb_->getByPattern(variable_entity, pattern_substring));
    }
    std::set_intersection(assign_result.begin(), assign_result.end(), assign_param->getContext().begin(),
                          assign_param->getContext().end(),
                          std::inserter(intersected_results, intersected_results.begin()));
  }
  if (left_param->getType() == EntityType::kString) {
    intersected_results = pkb_->getByPattern(new VariableEntity(left_param->getString()), pattern_substring);
  }
  assign_param->setContext(intersected_results);
}
