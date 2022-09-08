#include "evaluation_strategy.h"

/*
 * Factory method
 */
EvaluationStrategy *EvaluationStrategy::getStrategy(IPKBGetter *pkb, QueryClause &query_clause) {
  switch (query_clause.getClauseType()) {
    case (ClauseType::kSuchThat): return new SuchThatStrategy(pkb, query_clause);
    case (ClauseType::kPattern): return new PatternStrategy(pkb, query_clause);
    default: throw std::runtime_error("Invalid clause type");
  }
}

/*
 * Evaluation strategies.
 * TODO(howtoosee): Implement the evaluation strategies.
 */
Result SuchThatStrategy::evaluate() {
  QuerySynonym placeholder_syn("placeholder");

  return Result::empty(placeholder_syn);
}
Result PatternStrategy::evaluate() {
  QuerySynonym placeholder_syn("placeholder");

  return Result::empty(placeholder_syn);
}
