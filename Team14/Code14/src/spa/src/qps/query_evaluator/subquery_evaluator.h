#pragma once

#include "evaluation_strategy.h"

class SubQueryEvaluator {
 private:
  IPKBQuerier *pkb_;

  EvaluationStrategy *strategy_;

 public:
  SubQueryEvaluator(IPKBQuerier *pkb, QueryClause &query_clause_);

  Result evaluate();
};
