#include "query_evaluator.h"
#include "spdlog/spdlog.h"

QueryDeclarationPointerUnorderedSet QueryEvaluator::copyDeclarations() {
  Query &query = this->query_;
  std::vector<QueryDeclaration *> query_declarations = query.getDeclarations();
  this->declarations_ = std::unordered_set<QueryDeclaration *,
                                           QueryDeclarationHashFunction,
                                           QueryDeclarationPointerEquality>(
      query_declarations.begin(), query_declarations.end()
  );
  return this->getDeclarationAsSet();
}

QueryDeclarationPointerUnorderedSet QueryEvaluator::fetchContext() {
  this->copyDeclarations();

  for (auto *declaration : this->declarations_) {
    std::unordered_set<Entity *, EntityHashFunction, EntityPointerEquality> query_declaration_context_set;
    query_declaration_context_set = this->pkb_->getEntities(declaration->getType());
    declaration->setContext(query_declaration_context_set);
  }
  return this->getDeclarationAsSet();
}

QueryDeclarationPointerUnorderedSet QueryEvaluator::evaluateSubQueries() {
  std::vector<QueryClause *> subquery_clauses = this->query_.getQueryCall().getClauseVector();

  for (auto *subquery_clause : subquery_clauses) {
    SubQueryEvaluator subquery_evaluator = SubQueryEvaluator(this->pkb_, subquery_clause);
    subquery_evaluator.evaluate();
  }
  return this->getDeclarationAsSet();
}

Result *QueryEvaluator::evaluate() {
  this->fetchContext();

  // Query declaration for whose results are to be returned.
  QueryDeclaration *called_declaration = this->query_.getQueryCall().getDeclaration();

  // If there are no sub-queries, return the context of the selected query declaration.
  // Else, evaluate sub-queries first.
  if (this->query_.hasSubClauses()) { this->evaluateSubQueries(); }

  QuerySynonym *synonym = called_declaration->getSynonym();
  std::unordered_set<Entity *, EntityHashFunction, EntityPointerEquality> context = called_declaration->getContext();
  return new Result(synonym, context);
}

QueryDeclarationPointerUnorderedSet QueryEvaluator::getDeclarationAsSet() {
  QueryDeclarationPointerUnorderedSet declaration_set;

  std::copy(this->declarations_.begin(), this->declarations_.end(),
            std::inserter(declaration_set, declaration_set.begin()));
  return declaration_set;
}
