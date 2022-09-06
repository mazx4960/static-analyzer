#include "query_evaluator.h"

Result QueryEvaluator::evaluate() {
  // Query declaration for whose results are to be returned.
  QueryDeclaration *selected_declaration = this->query_.getQueryCall().getDeclaration();
  QuerySynonym query_synonym = selected_declaration->getSynonym();

  // Declarations in the query.
  std::vector<QueryDeclaration *> query_declarations = this->query_.getDeclarations();

  // Sub-queries that need to be processed.
  std::vector<QueryClause> subquery_clauses = this->query_.getQueryCall().getClauseVector();

  for (auto *declaration : query_declarations) {
    QuerySynonym declaration_synonym = declaration->getSynonym();
    Entity variable_entity = declaration->getEntity();

    PKBQuery pkb_query = *PKBQuery::getQuery(variable_entity);
    pkb_query.setSynonym(declaration_synonym);

    Result query_result = this->pkb_->get(pkb_query);
    std::unordered_set<std::string> results_set = query_result.get_results_set();

    declaration->setContext(results_set);
  }

  if (subquery_clauses.empty()) {
    // Selected QueryDeclaration is not in initialised.
    std::unordered_set<std::string> results_set = selected_declaration->getContext();
    return Result(query_synonym, results_set);
  }

  // TODO(howtoosee): implement projection and aggregation
  return Result::empty(query_synonym);

}

