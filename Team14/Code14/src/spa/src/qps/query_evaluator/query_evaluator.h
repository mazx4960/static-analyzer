// Copyright 2022 CS3203 Team14. All rights reserved.
#pragma once

#include <iterator>
#include <unordered_map>
#include <unordered_set>

#include "evaluation_strategy.h"
#include "qps/pql/query.h"
#include "result_projector.h"
#include "context.h"
#include "qps/query_evaluator/database/database.h"
#include "query_optimiser.h"

class QueryEvaluator {
 private:
  Query query_;
  IPKBQuerier *pkb_;
  Context *ctx_;
  Database *database_;
  /**
   * Calls PKB to fetch context for each query declaration.
   */
  void InitContext();

  /**
   * Evaluate sub-queries.
   */
  void EvaluateSubQueries();

 public:
  QueryEvaluator(IPKBQuerier *pkb, Query &query) : pkb_(pkb), query_(query) {
    this->ctx_ = new Context();
    this->database_ = new Database(this->ctx_, this->query_.getSynonymDeclarations());
  };

  /**
   * Evaluate query.
   * @return ElemResult of query with set of Entity instances.
   */
  Result *Evaluate();
};
