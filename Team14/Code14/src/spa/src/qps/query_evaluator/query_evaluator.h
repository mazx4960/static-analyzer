// Copyright 2022 CS3203 Team14. All rights reserved.
#pragma once

#include <iterator>
#include <unordered_map>
#include <unordered_set>

#include "evaluation_strategy.h"
#include "qps/pql/query.h"
#include "subquery_evaluator.h"

class QueryEvaluator {
 protected:
  Query query_;

  IPKBQuerier *pkb_;

  std::vector<Result> partial_results_list_;

  std::unordered_set<QueryDeclaration *,
                     QueryDeclarationHashFunction,
                     QueryDeclarationPointerEquality> declarations_;

  std::unordered_set<QueryDeclaration *,
                     QueryDeclarationHashFunction,
                     QueryDeclarationPointerEquality> copyDeclarations();
  std::unordered_set<QueryDeclaration *,
                     QueryDeclarationHashFunction,
                     QueryDeclarationPointerEquality> fetchContext();
  std::unordered_set<QueryDeclaration *,
                     QueryDeclarationHashFunction,
                     QueryDeclarationPointerEquality> evaluateSubQueries();
  std::unordered_set<QueryDeclaration *,
                     QueryDeclarationHashFunction,
                     QueryDeclarationPointerEquality> getDeclarationAsSet();

 public:
  QueryEvaluator(IPKBQuerier *pkb, Query &query) : pkb_(pkb), query_(query) {};

  Result *evaluate();
};
