#pragma once

#include <unordered_map>
#include "qps/pql/query.h"
#include "qps/query_evaluator/context.h"
#include "types.h"
#include "pkb/pkb.h"


class QueryOptimiser {
 private:
  Context *context_;
  std::unordered_map<EntityType, int> simple_stats_;
  std::unordered_map<QuerySynonym*, int> synonym_usage_;

  struct {
    bool operator()(QueryClause *first, QueryClause *second) const {
      return first->getWeight() < second->getWeight();
    }
  } clause_comparator_;

  void updateSynonymUsage(QueryClause *clause);
  void updateSynonymUsage(const ClauseVector &clauses);
  void updateSynonymUsage(QueryReference *query_reference);
  int getReferenceUsage(QueryReference *query_reference);
  EntitySet getReferenceContext(QueryReference *query_reference);
  double calculateInitialClauseWeight(QueryClause *query_clause);
  QueryClause *updateInitialWeight(QueryClause *clause);
  void updateClauseWeight(const ClauseVector& clauses);
  static void updateClauseWeight(QueryClause *clause);
  static void updateSynonymWeight(QueryClause *query_clause);
  static void updateSynonymWeight(const ClauseVector& clauses);
  void updateInitialWeights(const ClauseVector &clauses);
 public:
  QueryOptimiser() = default;
  Query Optimise(const Query& query, Context *context, IPKBQuerier *pkb);
};

