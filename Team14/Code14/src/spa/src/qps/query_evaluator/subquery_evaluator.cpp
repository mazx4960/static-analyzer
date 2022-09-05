#include "subquery_evaluator.h"
SubQueryEvaluator::SubQueryEvaluator(PKBInterface &pkb_interface, QueryClause &query_clause_) : query_clause_(
    query_clause_) {
  this->strategy_ = EvaluationStrategy::getStrategy(pkb_interface, query_clause_);
}

Result SubQueryEvaluator::evaluate() {
  return this->strategy_->evaluate();
}
