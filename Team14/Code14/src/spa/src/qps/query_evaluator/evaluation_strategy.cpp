#include "evaluation_strategy.h"

EvaluationStrategy *EvaluationStrategy::getStrategy(PKBInterface &pkb_interface, QueryCall &query_call) {
  return new SuchThatStrategy(pkb_interface, query_call);
}

Result SuchThatStrategy::evaluate() {
  QuerySynonym placeholder_syn("placeholder");

  return Result::empty(placeholder_syn);
}
