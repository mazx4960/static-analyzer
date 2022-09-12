// Copyright 2022 CS3203 Team14. All rights reserved.
#pragma once

#include <unordered_map>
#include <unordered_set>

#include "commons/pkb_query.h"
#include "evaluation_strategy.h"
#include "qps/pql/query/query.h"
#include "subquery_evaluator.h"

class QueryEvaluator {
 private:
  Query query_;

  IPKBQuerier *pkb_;

  std::vector<Result> partial_results_list_;

  std::vector<QueryDeclaration*> declarations_;

 public:
  QueryEvaluator(IPKBQuerier *pkb, Query &query) : pkb_(pkb), query_(query) {};

  Result evaluate();
};
