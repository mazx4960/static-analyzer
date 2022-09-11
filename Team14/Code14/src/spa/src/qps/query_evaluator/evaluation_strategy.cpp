#include "evaluation_strategy.h"

/*
 * Factory method
 */
EvaluationStrategy *EvaluationStrategy::getStrategy(IPKBQuerier *pkb, QueryClause &query_clause) {
  switch (query_clause.getClauseType()) {
    case ClauseType::kSuchThat: return new SuchThatStrategy(pkb, static_cast<SuchThatClause &> (query_clause));
    case ClauseType::kPattern:return new PatternStrategy(pkb, static_cast<PatternClause &> (query_clause));
    default: throw EvaluationStrategyCreationError("Invalid query clause type");
  }
}

// TODO(howtoosee) implement SuchThat strategy
void SuchThatStrategy::evaluate() {
  // TODO(howtoosee, KelvinSoo) split strategy into 3 types: first is synonym, second is synonym, both are synonyms.

  SuchThatType query_type = this->clause_.getSuchThatType();
  RsType rs_type; // TODO(howtoosee) convert SuchThatType query_type to RsType rs_type

  QueryDeclaration *first_param = this->clause_.getFirst();
  QueryDeclaration *second_param = this->clause_.getSecond();

  QueryDeclaration *first_to_evaluate;
  QueryDeclaration *second_to_evaluate;

  if (first_param->getContext().size() <= second_param->getContext().size()) {
    first_to_evaluate = first_param;
    second_to_evaluate = second_param;
  } else {
    first_to_evaluate = second_param;
    second_to_evaluate = first_param;
  }

  for (auto *context_entity : first_to_evaluate->getContext()) {
    std::unordered_set<Entity *> second_results = this->pkb_->getByRelationship(rs_type, context_entity, false);
    if (second_results.empty()) {
      first_to_evaluate->getContext().erase(context_entity);
    }
    std::unordered_set<Entity *> results;

    std::set_intersection(second_results.begin(), second_results.end(),
                          second_to_evaluate->getContext().begin(), second_to_evaluate->getContext().end(),
                          std::inserter(results, results.begin()));
  }
}

void PatternStrategy::evaluate() {
  QuerySynonym placeholder_syn("placeholder");
}
