#include "query_evaluator.h"

void QueryEvaluator::fetchContext() {
  // Query declaration for whose results are to be returned.

  QueryDeclaration *selected_declaration = this->query_.getQueryCall().getDeclaration();
  QuerySynonym query_synonym = selected_declaration->getSynonym();


  // Declarations in the query.
  std::vector<QueryDeclaration *> query_declarations = this->query_.getDeclarations();

  for (QueryDeclaration *declaration : query_declarations) {

    QuerySynonym declaration_synonym = declaration->getSynonym();
    Entity *variable_entity = declaration->getEntity();

    PKBQuery pkb_query = *PKBQuery::getQuery(*variable_entity);
    pkb_query.setSynonym(declaration_synonym);

    Result query_result = this->pkb_->get(pkb_query);
    this->context_.insert({*declaration, query_result.get_results_set()});
  }
}

Result QueryEvaluator::evaluate() {
  this->fetchContext();

  // Query declaration for whose results are to be returned.
  QueryDeclaration *called_declaration = this->query_.getQueryCall().getDeclaration();
  QuerySynonym query_synonym = called_declaration->getSynonym();

  // Sub-queries that need to be processed.
  std::vector<QueryClause> subquery_clauses = this->query_.getQueryCall().getClauseVector();

  if (subquery_clauses.empty()) {
    return Result(query_synonym, this->context_.at(*called_declaration));
  }


  // TODO(howtoosee): implement projection and aggregation
  return Result::empty(query_synonym);

}

