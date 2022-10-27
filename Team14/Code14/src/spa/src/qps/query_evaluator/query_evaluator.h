// Copyright 2022 CS3203 Team14. All rights reserved.
#pragma once

#include <iterator>
#include <unordered_map>
#include <unordered_set>

#include "evaluation_strategy.h"
#include "qps/pql/query.h"
#include "result_projector.h"
#include "context.h"

class QueryEvaluator {
 private:
  Clauses getSortedQueries();
  static QueryClause *updateWeight(QueryClause *clause);
  static double calculateWeight(int first_usage_count, int second_usage_count);

 protected:
  Query query_;

  IPKBQuerier *pkb_;

  Context *ctx_;

  /**
   * Calls PKB to fetch context for each query declaration.
   */
  void InitContext();

  /**
   * Evaluate sub-queries.
   * @return vector of SubqueryResults.
   */
  std::vector<SubqueryResult> EvaluateSubqueries();

 public:
  QueryEvaluator(IPKBQuerier *pkb, Query &query) : pkb_(pkb), query_(query) {
    this->ctx_ = new Context();
  };

  /**
   * Evaluate query.
   * @return ElemResult of query with set of Entity instances.
   */
  Result *Evaluate();
};
