#include "evaluation_strategy.h"

EvaluationStrategy *EvaluationStrategy::getStrategy(PKBInterface &pkb_interface, Query &query) {
  if (query.getDeclarations().size() == 1) {
    return new SelectStrategy(pkb_interface, query);
  }

  return new SuchThatStrategy(pkb_interface, query);
}

Result SelectStrategy::evaluate() {
  QuerySynonym syn = this->query_.getQueryCall().getDeclaration().getSynonym();

  PKBQuery pkb_query;
  pkb_query.setSynonym(syn);

  return this->pkb_interface_.get(pkb_query);
}

Result SuchThatStrategy::evaluate() {
  QuerySynonym placeholder_syn("placeholder");
  std::vector<std::string> placeholder_results(1);

  return Result(placeholder_syn, placeholder_results);
}
