#pragma once

#include "evaluation_strategy.h"

class SubQueryEvaluator {
 private:
  QueryClause query_clause_;

  EvaluationStrategy *strategy_;

 public:
  explicit SubQueryEvaluator(PKBInterface &pkb_interface, QueryClause &query_clause_);
  Result evaluate();

};
