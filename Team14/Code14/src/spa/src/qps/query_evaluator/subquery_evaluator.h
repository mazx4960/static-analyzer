#pragma once

#include "evaluation_strategy.h"

class SubQueryEvaluator {
 private:
  IPKBQuery *pkb_;

  EvaluationStrategy *strategy_;

 public:
  SubQueryEvaluator(IPKBQuery *pkb, QueryClause &query_clause_);

  Result evaluate();
};
