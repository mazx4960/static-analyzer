#include "evaluation_strategy.h"

EvaluationStrategy EvaluationStrategy::getStrategy(PKBInterface &pkb_interface, Query &query) {
  if (query.declarationVector.size() == 1) {
    return SelectStrategy(pkb_interface, query);
  }

  return SuchThatStrategy(pkb_interface, query);
}

Result SelectStrategy::evaluate() {
  Synonym syn = this->query_.query_declaration.synonym;

  PKBQuery pkb_query;
  pkb_query.setSynonym(syn);

  return this->pkb_interface_.get(pkb_query);
}

Result SuchThatStrategy::evaluate() {
  Synonym placeholder_syn;
  std::vector<std::string> placeholder_results(1);
  return Result(placeholder_syn, placeholder_results);
}
