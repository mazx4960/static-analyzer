// Copyright 2022 CS3203 Team14. All rights reserved.
#pragma once

#include <iterator>
#include <unordered_map>
#include <unordered_set>

#include "evaluation_strategy.h"
#include "qps/pql/query.h"
#include "result_projector.h"
#include "subquery_evaluator.h"

using SynonymReferencePointerUnorderedSet = std::unordered_set<SynonymReference *,
                                                               SynonymReferenceHashFunction,
                                                               SynonymReferencePointerEquality>;

class QueryEvaluator {
 private:
  Clauses getSortedQueries();
  QueryClause *updateWeight(QueryClause *clause);
  static double calculateWeight(double contex_size, int usage_count);

 protected:
  Query query_;

  IPKBQuerier *pkb_;

  SynonymReferencePointerUnorderedSet declarations_;

  /**
   * Copy declarations from Query into a set.
   * @return set of query declarations.
   */
  SynonymReferencePointerUnorderedSet copyDeclarations();

  /**
   * Calls PKB to fetch context for each query declaration.
   * @return set of query declarations.
   */
  SynonymReferencePointerUnorderedSet fetchContext();

  /**
   * Copies query declarations into a set.
   * Used to ensure immutability.
   * @return duplicated set of query declarations.
   */
  SynonymReferencePointerUnorderedSet getDeclarationAsSet();

  /**
   * Evaluate sub-queries.
   * @return vector of SubqueryResults.
   */
  std::vector<SubqueryResult> evaluateSubqueries();

 public:
  QueryEvaluator(IPKBQuerier *pkb, Query &query) : pkb_(pkb), query_(query) {
  };

  /**
   * Evaluate query.
   * @return ElemResult of query with set of Entity instances.
   */
  Result *evaluate();
};
