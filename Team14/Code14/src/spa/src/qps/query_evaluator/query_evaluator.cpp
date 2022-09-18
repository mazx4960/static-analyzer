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
  std::vector<QueryDeclaration *> query_declarations = query.getDeclarations();
  this->declarations_ = std::unordered_set<QueryDeclaration *,
                                           QueryDeclarationHashFunction,
                                           QueryDeclarationPointerEquality>(
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
    query_declaration_context_set = this->pkb_->getEntities(declaration->getType());
    declaration->setContext(query_declaration_context_set);
  }
  return this->getDeclarationAsSet();
}

/**
 * Evaluate each subquery.
 * Checks if subquery has any results, if not, set has_result_ to false.
 * @return set of query declarations.
 */
QueryDeclarationPointerUnorderedSet QueryEvaluator::evaluateSubQueries() {
  std::vector<QueryClause *> subquery_clauses = this->query_.getQueryCall().getClauseVector();

  for (auto *subquery_clause : subquery_clauses) {
    SubQueryEvaluator subquery_evaluator = SubQueryEvaluator(this->pkb_, subquery_clause);
    SubqueryResult subquery_result = subquery_evaluator.evaluate();
    // If subquery has no results, then overall query has no results, terminate early
    if (!subquery_result.empty()) {
      this->has_result_ = false;
      break;
    }
  }
  return this->getDeclarationAsSet();
}

/**
 * Evaluate query.
 * @return Result of query with set of Entity instances.
 */
Result *QueryEvaluator::evaluate() {
  this->fetchContext();

  // Query declaration for whose results are to be returned.
  QueryDeclaration *called_declaration = this->query_.getQueryCall().getDeclaration();

  // If there are no sub-queries, return the context of the selected query declaration.
  // Else, evaluate sub-queries first.
  if (this->query_.hasSubClauses()) { this->evaluateSubQueries(); }

  QuerySynonym *synonym = called_declaration->getSynonym();
  std::unordered_set<Entity *, EntityHashFunction, EntityPointerEquality> context = called_declaration->getContext();

  if (this->has_result_) {
    return new Result(synonym, context);
  }
  return Result::empty(synonym);
}
