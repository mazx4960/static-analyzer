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
 * Evaluate query.
 * @return Result of query with set of Entity instances.
 */
Result *QueryEvaluator::evaluate() {
  this->fetchContext();

  // Query declaration for whose subquery_results are to be returned.
  QueryDeclaration *called_declaration = this->query_.getQueryCall().getDeclaration();
  QuerySynonym *synonym = called_declaration->getSynonym();

  std::vector<QueryClause *> subquery_clauses = this->query_.getQueryCall().getClauseVector();

  std::vector<SubqueryResult> subquery_results;
  for (auto *subquery_clause : subquery_clauses) {
    SubQueryEvaluator subquery_evaluator = SubQueryEvaluator(this->pkb_, subquery_clause);
    SubqueryResult subquery_result = subquery_evaluator.evaluate();
    subquery_results.push_back(subquery_result);
    // If subquery has no subquery_results, then overall query has no subquery_results, terminate early
    if (subquery_result.empty()) {
      this->has_result_ = false;
      return Result::empty();
    }
  }
  EntityPointerUnorderedSet candidates = called_declaration->getContext();
  switch (subquery_results.size()) {
    case 0:
      // Just return all possible results
      return new Result(synonym, candidates);
    case 1:
      if (subquery_results[0].uses(called_declaration)) {
        return new Result(synonym, subquery_results[0].GetColumn(synonym));
      }
      return new Result(synonym, candidates);
    case 2:
      std::vector<QueryDeclaration *> common_synonyms = subquery_results[0].getCommonSynonyms(subquery_results[1]);
      switch (common_synonyms.size()) {
        case 0: {
          for (auto res : subquery_results) {
            if (res.uses(called_declaration)) {
              candidates = EvaluationStrategy::intersect(candidates, res.GetColumn(synonym));
            }
          }
          return new Result(synonym, candidates);
        }
        case 1: {
          if (*common_synonyms[0]->getSynonym() == *synonym) {
            for (auto res : subquery_results) {
              if (res.uses(called_declaration)) {
                candidates = EvaluationStrategy::intersect(candidates, res.GetColumn(synonym));
              }
            }
            return new Result(synonym, candidates);
          }
          SubqueryResult join = subquery_results[0].Join(subquery_results[1]);
          if (join.uses(called_declaration)) { return new Result(synonym, join.GetColumn(synonym)); }
          return new Result(synonym, called_declaration->getContext());
        }
        case 2: {
          SubqueryResult intersection = subquery_results[0].Intersect(subquery_results[1]);
          if (intersection.uses(called_declaration)) { return new Result(synonym, intersection.GetColumn(synonym)); }
          return new Result(synonym, candidates);
        }
      }
  }
  return Result::empty();
}
