// Copyright 2022 CS3203 Team14. All rights reserved.
#pragma once

#include "evaluation_strategy.h"
#include "subquery_evaluator.h"

class QueryEvaluator {
 private:
  Query query_;

  PKBInterface pkb_interface_;

  std::vector<Result> partial_results_list_;

 public:
  QueryEvaluator(PKBInterface &pkb_interface, Query &query) : pkb_interface_(pkb_interface), query_(query) {};

  Result evaluate();
};
