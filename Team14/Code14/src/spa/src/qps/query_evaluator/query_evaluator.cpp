#include "query_evaluator.h"

SynonymReferencePointerUnorderedSet QueryEvaluator::getDeclarationAsSet() {
  SynonymReferencePointerUnorderedSet declaration_set;

  std::copy(this->declarations_.begin(), this->declarations_.end(),
            std::inserter(declaration_set, declaration_set.begin()));
  return declaration_set;
}

SynonymReferencePointerUnorderedSet QueryEvaluator::copyDeclarations() {
  Query &query = this->query_;
  std::vector<SynonymReference *> query_declarations = query.getSynonymDeclarations();
  this->declarations_ = SynonymReferencePointerUnorderedSet(query_declarations.begin(), query_declarations.end());
  return this->getDeclarationAsSet();
}

SynonymReferencePointerUnorderedSet QueryEvaluator::fetchContext() {
  this->copyDeclarations();

  for (auto *declaration : this->declarations_) {
    std::unordered_set<Entity *, EntityHashFunction, EntityPointerEquality> query_declaration_context_set;
    query_declaration_context_set = this->pkb_->getEntities(declaration->getEntityType());
    declaration->setContext(query_declaration_context_set);
  }
  return this->getDeclarationAsSet();
}

std::vector<SubqueryResult> QueryEvaluator::evaluateSubqueries() {
  std::vector<QueryClause *> subquery_clauses = this->query_.getClauses();
  std::vector<SubqueryResult> subquery_results_list;
  for (auto *clause : subquery_clauses) {
    SubQueryEvaluator subquery_evaluator = SubQueryEvaluator(this->pkb_, clause);
    SubqueryResult subquery_result = subquery_evaluator.evaluate();
    subquery_results_list.push_back(subquery_result);
  }
  return subquery_results_list;
}

Result *QueryEvaluator::evaluate() {
  this->fetchContext();

  // Query declarations for whose subquery_results are to be returned.
  std::vector<ElemReference *> called_declarations = this->query_.getQueryCall()->getReferences();

  // TODO(gabriel): Return value for the vector of references
  // TODO(gabriel): Update Result class to accept more than one selected declaration
  QuerySynonym *synonym = called_declarations.front()->getSynonymReference()->getSynonym();
  SynonymReference *called_declaration = called_declarations.front()->getSynonymReference();
  // Placeholders

  std::vector<SubqueryResult> subquery_results = this->evaluateSubqueries();
  auto *result_projector = new ResultProjector(called_declaration, subquery_results, pkb_);
  EntityPointerUnorderedSet result_context = result_projector->project();

  return new Result(synonym, result_context);
}
