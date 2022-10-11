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
 protected:
  Query query_;

  IPKBQuerier *pkb_;

  // Used to check if subquery clauses have any results, initially assumed to be true
  bool has_result_ = true;

  SynonymReferencePointerUnorderedSet declarations_;

  SynonymReferencePointerUnorderedSet copyDeclarations();
  SynonymReferencePointerUnorderedSet fetchContext();
  SynonymReferencePointerUnorderedSet getDeclarationAsSet();
  std::vector<SubqueryResult> evaluateSubqueries();

 public:
  QueryEvaluator(IPKBQuerier *pkb, Query &query) : pkb_(pkb), query_(query) {
  };

  Result *evaluate();
};
