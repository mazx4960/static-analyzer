#pragma once

#include <iterator>
#include <unordered_set>

#include "qps/result.h"
#include "pkb/pkb.h"
#include "qps/exceptions.h"
#include "qps/pql/query_clause.h"
#include "qps/query_evaluator/subquery_result.h"

using EntityPointerUnorderedSet = std::unordered_set<Entity *, EntityHashFunction, EntityPointerEquality>;

class EvaluationStrategy {
 protected:
  IPKBQuerier *pkb_;

  explicit EvaluationStrategy(IPKBQuerier *pkb) : pkb_(pkb) {};

 public:
  static EvaluationStrategy *getStrategy(IPKBQuerier *, QueryClause *);
  virtual SubqueryResult evaluate() = 0;
  EntityPointerUnorderedSet getCandidates(QueryDeclaration *);
  static bool shouldIntersect(QueryDeclaration *);
  static EntityPointerUnorderedSet intersect(const EntityPointerUnorderedSet &first, const EntityPointerUnorderedSet &second);
};

/*
 * Selection for such-that clauses
 */
class SuchThatStrategy : public EvaluationStrategy {
 private:
  SuchThatClause *clause_;

 public:
  SuchThatStrategy(IPKBQuerier *pkb, SuchThatClause *query_clause) : EvaluationStrategy(pkb), clause_(query_clause) {};
  SubqueryResult evaluate() override;
  EntityPointerUnorderedMap evaluateParameter(QueryDeclaration *, RsType, bool, const EntityPointerUnorderedSet &);
};

/*
 * Selection for pattern clauses
 */
class PatternStrategy : public EvaluationStrategy {
  PatternClause *clause_;

 public:
  PatternStrategy(IPKBQuerier *pkb, PatternClause *query_clause) : EvaluationStrategy(pkb), clause_(query_clause) {};
  SubqueryResult evaluate() override;
  EntityPointerUnorderedMap evaluateParameter(QueryDeclaration *, QueryDeclaration *, const EntityPointerUnorderedSet &);
};
