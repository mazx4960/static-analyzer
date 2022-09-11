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
std::unordered_set<Entity *> SuchThatStrategy::evaluate() {
  QuerySynonym placeholder_syn("placeholder");

  return std::unordered_set<Entity *>();
}

std::unordered_set<Entity *> PatternStrategy::evaluate() {
  QuerySynonym placeholder_syn("placeholder");

  return std::unordered_set<Entity *>();
}
