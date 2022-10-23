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
