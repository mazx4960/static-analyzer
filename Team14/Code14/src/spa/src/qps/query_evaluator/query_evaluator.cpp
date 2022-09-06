#include "query_evaluator.h"

Result QueryEvaluator::evaluate() {
  // Query declaration for whose results are to be returned.
  QueryDeclaration called_declaration = this->query_.getQueryCall().getDeclaration();
  QuerySynonym query_synonym = called_declaration.getSynonym();

  // Declarations in the query.
  std::vector<QueryDeclaration> query_declarations = this->query_.getDeclarations();

  // Sub-queries that need to be processed.
  std::vector<QueryClause> subquery_clauses = this->query_.getQueryCall().getClauseVector();

  for (QueryDeclaration &declaration : query_declarations) {

    QuerySynonym declaration_synonym = declaration.getSynonym();
    Entity variable_entity = declaration.getEntity();

    PKBQuery pkb_query = *PKBQuery::getQuery(variable_entity);
    pkb_query.setSynonym(declaration_synonym);

    Result query_result = this->pkb_->getResult(pkb_query);
    this->context_.insert({declaration, query_result.get_results_set()});
  }

  if (subquery_clauses.empty()) {
    return Result(query_synonym, this->context_.at(called_declaration));
  }


  // TODO(howtoosee): implement projection and aggregation
  return Result::empty(query_synonym);

}

