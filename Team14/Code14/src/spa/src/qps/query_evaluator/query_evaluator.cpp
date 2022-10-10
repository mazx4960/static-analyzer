#include "query_evaluator.h"
#include "spdlog/spdlog.h"

/**
 * Copies query declarations into a set.
 * Used to ensure immutability.
 * @return duplicated set of query declarations.
 */
QueryDeclarationPointerUnorderedSet QueryEvaluator::getDeclarationAsSet() {
  QueryDeclarationPointerUnorderedSet declaration_set;

  std::copy(this->declarations_.begin(), this->declarations_.end(),
            std::inserter(declaration_set, declaration_set.begin()));
  return declaration_set;
}

/**
 * Copy declarations from Query into a set.
 * @return set of query declarations.
 */
QueryDeclarationPointerUnorderedSet QueryEvaluator::copyDeclarations() {
  Query &query = this->query_;
  std::vector<SynonymReference *> query_declarations = query.getSynonymDeclarations();
  this->declarations_ = std::unordered_set <QueryReference *,
                                            QueryDeclarationHashFunction,
                                            QueryDeclarationPointerEquality > (
          query_declarations.begin(), query_declarations.end()
      );
  return this->getDeclarationAsSet();
}

/**
 * Calls PKB to fetch context for each query declaration.
 * @return set of query declarations.
 */
QueryDeclarationPointerUnorderedSet QueryEvaluator::fetchContext() {
  this->copyDeclarations();

  for (auto *declaration : this->declarations_) {
    std::unordered_set<Entity *, EntityHashFunction, EntityPointerEquality> query_declaration_context_set;
    query_declaration_context_set = this->pkb_->getEntities(declaration->getEntityType());
    declaration->setContext(query_declaration_context_set);
  }
  return this->getDeclarationAsSet();
}

/**
 * Evaluate sub-queries.
 * @return vector of SubqueryResults.
 */
std::vector<SubqueryResult> QueryEvaluator::evaluateSubqueries() {
  std::vector<QueryClause *> subquery_clauses = this->query_.getQueryCall()->getClauseVector();
  std::vector<SubqueryResult> subquery_results_list;
  for (auto *clause : subquery_clauses) {
    SubQueryEvaluator subquery_evaluator = SubQueryEvaluator(this->pkb_, clause);
    SubqueryResult subquery_result = subquery_evaluator.evaluate();
    subquery_results_list.push_back(subquery_result);
  }
  return subquery_results_list;
}

/**
 * Evaluate query.
 * @return Result of query with set of Entity instances.
 */
Result *QueryEvaluator::evaluate() {
  this->fetchContext();

  // Query declaration for whose subquery_results are to be returned.
  SynonymReference *called_declaration = this->query_.getQueryCall()->getReference();
  QuerySynonym *synonym = called_declaration->getSynonym();

  std::vector<SubqueryResult> subquery_results = this->evaluateSubqueries();
  auto *result_projector = new ResultProjector(called_declaration, subquery_results, pkb_);
  EntityPointerUnorderedSet result_context = result_projector->project();

  return new Result(synonym, result_context);
}
