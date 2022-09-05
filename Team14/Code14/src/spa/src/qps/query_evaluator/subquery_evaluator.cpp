#include "subquery_evaluator.h"
SubQueryEvaluator::SubQueryEvaluator(PKBInterface &pkb_interface, QueryCall &query_call) : query_call_(query_call) {
  this->strategy_ = EvaluationStrategy::getStrategy(pkb_interface, query_call);
}

Result SubQueryEvaluator::evaluate() {
  return this->strategy_->evaluate();
}
