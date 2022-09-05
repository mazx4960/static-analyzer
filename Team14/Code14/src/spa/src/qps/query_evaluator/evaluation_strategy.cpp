#include "evaluation_strategy.h"

/*
 * Factory method
 */
EvaluationStrategy *EvaluationStrategy::getStrategy(PKBInterface &pkb_interface, QueryClause &query_clause) {
  switch (query_clause.getClauseType()) {
    case (ClauseType::kSuchThat): return new SuchThatStrategy(pkb_interface, query_clause);
    case (ClauseType::kPattern): return new PatternStrategy(pkb_interface, query_clause);
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
