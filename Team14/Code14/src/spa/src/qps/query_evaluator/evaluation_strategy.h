#pragma once

#include <unordered_set>
#include "pkb/pkb.h"
#include "pkb/entity/result.h"
#include "qps/pql/query_clause/query_clause.h"

class EvaluationStrategy {
 protected:
  IPKBQuery *pkb_;

  explicit EvaluationStrategy(IPKBQuery *pkb) : pkb_(pkb) {};
 public:
  static EvaluationStrategy *getStrategy(IPKBQuery *, QueryClause &);
  virtual Result evaluate() = 0;
};

/*
 * Selection for such-that clauses
 */
class SuchThatStrategy : public EvaluationStrategy {
 private:
  SuchThatClause clause_;

 public:
  SuchThatStrategy(IPKBQuery *pkb, SuchThatClause &query_clause)
      : EvaluationStrategy(pkb), clause_(query_clause) {};

  Result evaluate() override;
};

class PatternStrategy : public EvaluationStrategy {
  PatternClause clause_;

 public:
  PatternStrategy(IPKBQuery *pkb, PatternClause &query_clause)
      : EvaluationStrategy(pkb), clause_(query_clause) {};

  Result evaluate() override;
};