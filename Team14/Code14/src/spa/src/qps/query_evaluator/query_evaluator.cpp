#include "query_evaluator.h"

void QueryEvaluator::fetchContext() {
  // Query declaration for whose results are to be returned.
  QueryDeclaration *selected_declaration = this->query_.getQueryCall().getDeclaration();
  QuerySynonym query_synonym = selected_declaration->getSynonym();

  // Declarations in the query.
  std::vector<QueryDeclaration *> query_declarations = this->query_.getDeclarations();

  for (QueryDeclaration *declaration : query_declarations) {
    QuerySynonym declaration_synonym = declaration->getSynonym();
    Entity *declaration_var_entity = declaration->getEntity();
    PKBEntityQuery pkb_query = *PKBEntityQuery::getQuery(*declaration_var_entity, declaration_synonym);

    Result query_declaration_context = this->pkb_->getResults(pkb_query);
    this->context_.insert({*declaration, query_declaration_context.get_results_set()});
  }
}

std::vector<Result> QueryEvaluator::evaluateSubQueries() {
  std::vector<QueryClause> subquery_clauses = this->query_.getQueryCall().getClauseVector();

  for (QueryClause subquery_clause : subquery_clauses) {
    SubQueryEvaluator subquery_evaluator = SubQueryEvaluator(this->pkb_, subquery_clause);
    Result subquery_result = subquery_evaluator.evaluate();
    this->partial_results_list_.push_back(subquery_result);
  }

  return this->partial_results_list_;
}

Result QueryEvaluator::projectResults(std::vector<Result> &partial_results_list) {
  QueryDeclaration *called_declaration = this->query_.getQueryCall().getDeclaration();
  QuerySynonym query_synonym = called_declaration->getSynonym();

  return Result::empty(query_synonym);

}

Result QueryEvaluator::evaluate() {
  this->fetchContext();

  // Query declaration for whose results are to be returned.
  QueryDeclaration *called_declaration = this->query_.getQueryCall().getDeclaration();
  QuerySynonym query_synonym = called_declaration->getSynonym();

  // No sub-queries, return the context of the selected query declaration.
  if (!this->query_.hasSubClauses()) {
    return Result(query_synonym, this->context_.at(*called_declaration));
  }

  // Evaluate sub-queries.
  std::vector<Result> partial_results = this->evaluateSubQueries();

  // Project results.
  Result final_result = this->projectResults(partial_results);
  return final_result;
}

