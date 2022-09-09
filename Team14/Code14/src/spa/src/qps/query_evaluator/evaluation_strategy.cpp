#include "evaluation_strategy.h"

/*
 * Factory method
 */
EvaluationStrategy *EvaluationStrategy::getStrategy(IPKBGetter *pkb, QueryClause &query_clause) {
  switch (query_clause.getClauseType()) {
    case ClauseType::kSuchThat: return new SuchThatStrategy(pkb, static_cast<SuchThatClause &> (query_clause));
    case ClauseType::kPattern:return new PatternStrategy(pkb, static_cast<PatternClause &> (query_clause));
    default: throw std::runtime_error("Invalid query clause type");
  }
}

// TODO(howtoosee) implement SuchThat strategy
Result SuchThatStrategy::evaluate() {
  QuerySynonym placeholder_syn("placeholder");

  return Result::empty(placeholder_syn);
}

Result PatternStrategy::evaluate() {
  QuerySynonym placeholder_syn("placeholder");
  Pattern pattern = this->clause_.getPattern();

  PKBPatternQuery pkb_query = *PKBPatternQuery::getQuery(pattern);

  // TODO(howtoosee, leeenen) waiting for PKB get() method
  //Result result = this->pkb_->get(pkb_query);
  Result result = Result::empty(placeholder_syn);

  return result;
}
