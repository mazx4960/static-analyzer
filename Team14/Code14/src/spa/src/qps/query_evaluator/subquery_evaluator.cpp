#include "subquery_evaluator.h"
SubQueryEvaluator::SubQueryEvaluator(IPKBGetter *pkb, QueryClause &query_clause) : pkb_(pkb) {
  this->strategy_ = EvaluationStrategy::getStrategy(pkb, query_clause);
}

Result SubQueryEvaluator::evaluate() {
  return this->strategy_->evaluate();
}
