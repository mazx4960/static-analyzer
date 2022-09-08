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

  IPKBQuery *pkb_;

  std::vector<Result> partial_results_list_;

  std::unordered_map<QueryDeclaration, std::unordered_set<std::string>, QueryDeclarationHashFunction> context_;

 public:
  QueryEvaluator(IPKBQuery *pkb, Query &query) : pkb_(pkb), query_(query) {};

  Result evaluate();
};
