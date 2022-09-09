#pragma once

#include <unordered_set>
#include "pkb/pkb.h"
#include "pkb/entity/result.h"
#include "qps/pql/query_clause/query_clause.h"

class EvaluationStrategy {
 protected:
  IPKBGetter *pkb_;

  explicit EvaluationStrategy(IPKBGetter *pkb) : pkb_(pkb) {};
 public:
  static EvaluationStrategy *getStrategy(IPKBGetter *, QueryClause &);
  virtual Result evaluate() = 0;
};

/*
 * Selection for such-that clauses
 */
class SuchThatStrategy : public EvaluationStrategy {
 private:
  SuchThatClause clause_;

 public:
  SuchThatStrategy(IPKBGetter *pkb, SuchThatClause &query_clause)
      : EvaluationStrategy(pkb), clause_(query_clause) {};

  Result evaluate() override;
};

class PatternStrategy : public EvaluationStrategy {
  PatternClause clause_;

 public:
  PatternStrategy(IPKBGetter *pkb, PatternClause &query_clause)
      : EvaluationStrategy(pkb), clause_(query_clause) {};

  Result evaluate() override;
};