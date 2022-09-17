#pragma once

#include <iterator>
#include <unordered_set>

#include "commons/result.h"
#include "pkb/pkb.h"
#include "qps/exceptions.h"
#include "qps/pql/query_clause.h"

using EntityPointerUnorderedSet = std::unordered_set<Entity *, EntityHashFunction, EntityPointerEquality>;

class EvaluationStrategy {
 protected:
  IPKBQuerier *pkb_;

  explicit EvaluationStrategy(IPKBQuerier *pkb) : pkb_(pkb) {};

 public:
  static EvaluationStrategy *getStrategy(IPKBQuerier *, QueryClause *);
  virtual bool evaluate() = 0;
  EntityPointerUnorderedSet getCandidates(QueryDeclaration *);
  static bool shouldIntersect(QueryDeclaration *);
  static EntityPointerUnorderedSet intersectContext(const EntityPointerUnorderedSet &,
                                                    const EntityPointerUnorderedSet &);
};

/*
 * Selection for such-that clauses
 */
class SuchThatStrategy : public EvaluationStrategy {
 private:
  SuchThatClause *clause_;

 public:
  SuchThatStrategy(IPKBQuerier *pkb, SuchThatClause *query_clause) : EvaluationStrategy(pkb), clause_(query_clause) {};
  bool evaluate() override;
  EntityPointerUnorderedSet evaluateParameter(QueryDeclaration *, RsType rs_type, bool invert_search);
};

/*
 * Selection for pattern clauses
 */
class PatternStrategy : public EvaluationStrategy {
  PatternClause *clause_;

 public:
  PatternStrategy(IPKBQuerier *pkb, PatternClause *query_clause) : EvaluationStrategy(pkb), clause_(query_clause) {};
  bool evaluate() override;
  EntityPointerUnorderedSet evaluateParameter(QueryDeclaration *, QueryDeclaration *);
};
