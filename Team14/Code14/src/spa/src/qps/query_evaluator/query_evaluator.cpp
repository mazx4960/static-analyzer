// Copyright 2022 CS3203 Team14. All rights reserved.

#include "query_evaluator.h"

Result QueryEvaluator::evaluate() {
  for (QueryCall &query_call : this->query_.getQueryCall()) {
    SubQueryEvaluator sub_query_evaluator = SubQueryEvaluator(this->pkb_interface_, query_call);

    Result partial_result = sub_query_evaluator.evaluate();
    this->partial_results_list_.push_back(partial_result);
  }

  if (this->partial_results_list_.size() == 1) {
    return this->partial_results_list_[0];
  }

  // TODO: implement projection and aggregation
  return this->partial_results_list_[0];
}
