#include "evaluation_strategy.h"

/**
 * Factory method
 */
EvaluationStrategy *EvaluationStrategy::getStrategy(IPKBQuerier *pkb, QueryClause* query_clause) {
  switch (query_clause->getClauseType()) {
    case ClauseType::kSuchThat: return new SuchThatStrategy(pkb, static_cast<SuchThatClause*>(query_clause));
    case ClauseType::kPattern: return new PatternStrategy(pkb, static_cast<PatternClause*>(query_clause));
    default: throw EvaluationStrategyCreationError("Invalid query clause type");
  }
}

// TODO(howtoosee) implement SuchThat strategy
void SuchThatStrategy::evaluate() {
  RsType rs_type;// TODO (howtoosee) translate clause type to rs_type

  QueryDeclaration *first_param = this->clause_->getFirst();
  QueryDeclaration *second_param = this->clause_->getSecond();

  if (first_param->getContext().size() <= second_param->getContext().size()) {
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
  std::unordered_set<Entity *> valid_entities_for_other_param;

  for (auto *context_entity : param_to_send->getContext()) {
    std::unordered_set<Entity *> valid_entities = this->pkb_->getByRelationship(rs_type, context_entity, invert_search);
    if (valid_entities.empty()) {
      param_to_send->getContext().erase(context_entity);
    } else {
      for (auto *entity : valid_entities) { valid_entities_for_other_param.insert(entity); }
    }
  }

  std::unordered_set<Entity *> intersected_results;

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
  std::string left_string = left_param->getString();
  std::string pattern_substring = right_param->getString();

  std::unordered_set<Entity *> valid_assign_entities = this->pkb_->getByPattern(left_string, pattern_substring);

  std::unordered_set<Entity *> intersected_results;
  std::set_intersection(valid_assign_entities.begin(), valid_assign_entities.end(), assign_param->getContext().begin(),
                        assign_param->getContext().end(),
                        std::inserter(intersected_results, intersected_results.begin()));

  assign_param->setContext(intersected_results);
}
