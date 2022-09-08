#pragma once

#include "evaluation_strategy.h"

class SubQueryEvaluator {
 private:
  IPKBGetter *pkb_;

  EvaluationStrategy *strategy_;

 public:
  SubQueryEvaluator(IPKBGetter *pkb, QueryClause &query_clause_);
  Result evaluate();
};
