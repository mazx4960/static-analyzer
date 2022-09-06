// Copyright 2022 CS3203 Team14. All rights reserved.
#pragma once

#include <unordered_map>
#include <unordered_set>

#include "evaluation_strategy.h"
#include "subquery_evaluator.h"
#include "pkb/entity/pkb_query.h"
#include "qps/pql/query/query.h"

class QueryEvaluator {
 private:
  Query query_;

  PKB *pkb_;

  std::unordered_set<QueryDeclaration, QueryDeclarationHashFunction> declarations_;

  std::vector<Result> partial_results_list_;

 public:
  QueryEvaluator(PKB *pkb, Query &query) : pkb_(pkb), query_(query) {};

  Result evaluate();
};
