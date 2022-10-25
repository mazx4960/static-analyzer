#include "subquery_evaluator.h"

#include "spdlog/spdlog.h"

SubQueryEvaluator::SubQueryEvaluator(IPKBQuerier *pkb, QueryClause *query_clause) {
  spdlog::info("Creating evaluation strategy for {}, weight: {}", query_clause->toString(), query_clause->getWeight());
  this->strategy_ = EvaluationStrategy::getStrategy(pkb, query_clause);
}

/**
 * Evaluate subquery clause.
 * @return true if query clause has results, false otherwise.
 */
SubqueryResult SubQueryEvaluator::evaluate() {
  return this->strategy_->evaluate();
}
