#include "query_evaluator.h"

Result QueryEvaluator::evaluate() {
  // Query declaration for whose results are to be returned.

  QueryDeclaration *selected_declaration = this->query_.getQueryCall().getDeclaration();
  QuerySynonym query_synonym = selected_declaration->getSynonym();

  // Declarations in the query.
  declarations_ = this->query_.getDeclarations();

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

