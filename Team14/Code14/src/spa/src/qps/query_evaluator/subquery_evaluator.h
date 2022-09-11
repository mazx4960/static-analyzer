#pragma once

#include <unordered_set>
#include "commons/entity.h"
#include "evaluation_strategy.h"

class SubQueryEvaluator {
 private:
  IPKBQuerier *pkb_;

  EvaluationStrategy *strategy_;

 public:
  SubQueryEvaluator(IPKBQuerier *pkb, QueryClause &query_clause_);

  std::unordered_set<QueryDeclaration *> evaluate();
};
