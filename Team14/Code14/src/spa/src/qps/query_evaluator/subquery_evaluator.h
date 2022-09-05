#pragma once

#include "evaluation_strategy.h"

class SubQueryEvaluator {
 private:
  QueryCall query_call_;

  EvaluationStrategy *strategy_;

 public:
  explicit SubQueryEvaluator(PKBInterface &pkb_interface, QueryCall &query_call);
  Result evaluate();

};
