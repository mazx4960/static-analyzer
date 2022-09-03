#include "evaluation_strategy.h"
#include "qps/pql/query_synonym/query_synonym.h"

EvaluationStrategy EvaluationStrategy::getStrategy(PKBInterface &pkb_interface, Query &query) {
  if (query.getDeclarations().size() == 1) {
    return SelectStrategy(pkb_interface, query);
  }

  return SuchThatStrategy(pkb_interface, query);
}

Result SelectStrategy::evaluate() {
  QuerySynonym syn = this->query_.getQueryCall().getDeclaration().getSynonym();

  PKBQuery pkb_query;
  pkb_query.setSynonym(syn);

  return this->pkb_interface_.get(pkb_query);
}

Result SuchThatStrategy::evaluate() {
  QuerySynonym placeholder_syn;
  std::vector<std::string> placeholder_results(1);
  return Result(placeholder_syn, placeholder_results);
}
