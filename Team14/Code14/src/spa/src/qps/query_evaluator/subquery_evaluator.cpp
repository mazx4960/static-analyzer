#include "subquery_evaluator.h"

SubQueryEvaluator::SubQueryEvaluator(IPKBQuerier *pkb, QueryClause &query_clause) : pkb_(pkb) {
  this->strategy_ = EvaluationStrategy::getStrategy(pkb, query_clause);
}

std::unordered_set<QueryDeclaration *> SubQueryEvaluator::evaluate() {
  return this->strategy_->evaluate();
}
