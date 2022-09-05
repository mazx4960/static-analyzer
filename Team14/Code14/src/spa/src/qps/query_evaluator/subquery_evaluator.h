#pragma once

#include "evaluation_strategy.h"

class SubQueryEvaluator {
 private:
  QueryClause query_clause_;

  EvaluationStrategy *strategy_;

 public:
  explicit SubQueryEvaluator(PKB *pkb, QueryClause &query_clause_);
  Result evaluate();
};
