#include "subquery_evaluator.h"
SubQueryEvaluator::SubQueryEvaluator(PKB *pkb, QueryClause &query_clause_) : query_clause_(
    query_clause_) {
  this->strategy_ = EvaluationStrategy::getStrategy(pkb, query_clause_);
}

Result SubQueryEvaluator::evaluate() {
  return this->strategy_->evaluate();
}
