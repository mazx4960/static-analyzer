// Copyright 2022 CS3203 Team14. All rights reserved.
#pragma once

#include <unordered_map>
#include <unordered_set>

#include "evaluation_strategy.h"
#include "qps/pql/query/query.h"
#include "subquery_evaluator.h"

class QueryEvaluator {
 private:
  Query query_;

  IPKBQuerier *pkb_;

  std::vector<Result> partial_results_list_;

  std::unordered_map<QuerySynonym, QueryDeclaration *, QuerySynonymHashFunction> declarations_;

  std::unordered_set<QueryDeclaration *> copyDeclarations();
  std::unordered_set<QueryDeclaration *> fetchContext();
  std::unordered_set<QueryDeclaration *> evaluateSubQueries();
  std::unordered_set<QueryDeclaration *> getDeclarationAsSet();

 public:
  QueryEvaluator(IPKBQuerier *pkb, Query &query) : pkb_(pkb), query_(query) {};

  Result evaluate();
};
