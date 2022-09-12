#include "subquery_evaluator.h"

SubQueryEvaluator::SubQueryEvaluator(IPKBQuerier *pkb, QueryClause* query_clause) {
  this->strategy_ = EvaluationStrategy::getStrategy(pkb, query_clause);
}

void SubQueryEvaluator::evaluate() {
  this->strategy_->evaluate();
}
