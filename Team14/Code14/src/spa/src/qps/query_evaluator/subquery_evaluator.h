#pragma once

#include <unordered_set>
#include "commons/entity.h"
#include "evaluation_strategy.h"

class SubQueryEvaluator {
 private:
  EvaluationStrategy *strategy_;

 public:
  SubQueryEvaluator(IPKBQuerier *pkb, QueryClause *query_clause);

  bool evaluate();
};
