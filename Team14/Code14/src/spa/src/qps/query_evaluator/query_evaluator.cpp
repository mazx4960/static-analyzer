#include "query_evaluator.h"

Result QueryEvaluator::evaluate() {
  // Query declaration for whose results are to be returned.

  QueryDeclaration *selected_declaration = this->query_.getQueryCall().getDeclaration();
  QuerySynonym query_synonym = selected_declaration->getSynonym();



  // Sub-queries that need to be processed.
  std::vector<QueryClause> subquery_clauses = this->query_.getQueryCall().getClauseVector();

  // Update context for each declaration
  for (QueryDeclaration* declaration : declarations_) {
    // TODO(howtoosee): update initial context_ in declaration
    // Something like: declaration->context_ = PKB.getAll(declaration->getType())
    // context_ is a unordered_set<Entity*>

  }

  // Set intersection for each clause
  for (QueryClause subquery_clause: subquery_clauses) {
    // TODO(howtoosee): create PKBQuery from subquery_clause in SIMPLE format Relationship(Entity, Entity)
    // Something like: PKB.setQuery(Relationship) or PKB.setQueries(vector<Relationship>)

    // TODO(howtoosee): get result from PKB as unordered_set<Entity*>

    // TODO(howtoosee): update context_ in declaration using set intersection of RESULT and context_

  }

  // TODO(howtoosee): unpack context_ of v from declarations_eg. Select "v"
  // Return the result

  // TODO(howtoosee): implement projection and aggregation
  return Result::empty(query_synonym);

}

void QueryEvaluator::fetchContext() {
  // Query declaration for whose results are to be returned.
  QueryDeclaration *selected_declaration = this->query_.getQueryCall().getDeclaration();
  QuerySynonym query_synonym = selected_declaration->getSynonym();

  // Declarations in the query.
  // Save a copy of all declarations locally
  declarations_ = this->query_.getDeclarations();

  for (QueryDeclaration *declaration : declarations_) {
    QuerySynonym declaration_synonym = declaration->getSynonym();
    // TODO(howtoosee): update initial context_ in declaration
    // Something like: declaration->context_ = PKB.getAll(declaration->getType())
    // context_ is a unordered_set<Entity*>


    PKBEntityQuery pkb_query = *PKBEntityQuery::getQuery(declaration->getType());
    Result query_declaration_context = this->pkb_->getResults(pkb_query);



    // get_results_entity_set() is a set of Entity
    // Dont need the hash map. You can save result into each declaration directly
    declaration->context_ = query_declaration_context.get_results_entity_set();
    // this->context_.insert({*declaration, query_declaration_context.get_results_set()});
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

  // We should remove synonym from PKB
  // QuerySynonym query_synonym = called_declaration->getSynonym();

  // No sub-queries, return the context of the selected query declaration.
  if (!this->query_.hasSubClauses()) {
    //return Result(query_synonym, this->context_.at(*called_declaration));


    // return Result(query_synonym, called_declaration->context_);

    // You can use this below instead, the synonym should be irrelevant, because we have the actual declaration to reference
    return Result(called_declaration->context_);
  }

  // Evaluate sub-queries.
  std::vector<Result> partial_results = this->evaluateSubQueries();

  // Project results.
  Result final_result = this->projectResults(partial_results);
  return final_result;
}