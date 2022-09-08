#pragma once

#include <unordered_set>
#include "pkb/pkb.h"
#include "pkb/entity/result.h"
#include "qps/pql/query_clause/query_clause.h"

class EvaluationStrategy {
 protected:
  IPKBGetter *pkb_;

  QueryClause query_call_;

  EvaluationStrategy(IPKBGetter *pkb, QueryClause &query_clause) : pkb_(pkb), query_call_(query_clause) {};
 public:
  static EvaluationStrategy *getStrategy(IPKBGetter *, QueryClause &);
  virtual Result evaluate() = 0;
};

/*
 * Selection for such-that clauses
 */
class SuchThatStrategy : public EvaluationStrategy {
 public:
  SuchThatStrategy(IPKBGetter *pkb, QueryClause &query_clause) : EvaluationStrategy(pkb, query_clause) {};
  Result evaluate() override;
};

class PatternStrategy : public EvaluationStrategy {
 public:
  PatternStrategy(IPKBGetter *pkb, QueryClause &query_clause) : EvaluationStrategy(pkb, query_clause) {};
  Result evaluate() override;
};