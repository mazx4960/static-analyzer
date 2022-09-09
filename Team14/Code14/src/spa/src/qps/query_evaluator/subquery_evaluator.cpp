#include "subquery_evaluator.h"

SubQueryEvaluator::SubQueryEvaluator(IPKBQuery *pkb, QueryClause &query_clause) : pkb_(pkb) {
  switch (query_clause.getClauseType()) {
    case ClauseType::kSuchThat:this->strategy_ = EvaluationStrategy::getStrategy(pkb, query_clause);
      break;
    case ClauseType::kPattern:this->strategy_ = EvaluationStrategy::getStrategy(pkb, query_clause);
      break;
    default: throw std::runtime_error("Invalid query clause type");
  }

}

Result SubQueryEvaluator::evaluate() {
  return this->strategy_->evaluate();
}

