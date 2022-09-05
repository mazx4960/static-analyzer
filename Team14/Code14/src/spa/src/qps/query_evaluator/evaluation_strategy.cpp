#include "evaluation_strategy.h"

EvaluationStrategy *EvaluationStrategy::getStrategy(PKBInterface &pkb_interface, QueryCall &query_call) {
  return new SuchThatStrategy(pkb_interface, query_call);
}

Result SuchThatStrategy::evaluate() {
  QuerySynonym placeholder_syn("placeholder");
  std::unordered_set<std::string> placeholder_results;

  return Result(placeholder_syn, placeholder_results);
}
