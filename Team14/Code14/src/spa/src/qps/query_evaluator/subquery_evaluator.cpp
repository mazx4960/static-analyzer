#include "subquery_evaluator.h"

SubQueryEvaluator::SubQueryEvaluator(IPKBQuerier *pkb, QueryClause *query_clause) {
  this->strategy_ = EvaluationStrategy::getStrategy(pkb, query_clause);
}

/**
 * Evaluate subquery clause.
 * @return true if query clause has results, false otherwise.
 */
bool SubQueryEvaluator::evaluate() {
  return this->strategy_->evaluate();
}
