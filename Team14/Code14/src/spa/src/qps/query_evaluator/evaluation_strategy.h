#pragma once

#include <unordered_set>

#include "commons/result.h"
#include "pkb/pkb.h"
#include "qps/pql/query_clause/query_clause.h"

class EvaluationStrategy {
 protected:
  IPKBQuerier *pkb_;

  explicit EvaluationStrategy(IPKBQuerier *pkb) : pkb_(pkb) {};
 public:
  static EvaluationStrategy *getStrategy(IPKBQuerier *, QueryClause &);
  virtual void evaluate() = 0;
};

/*
 * Selection for such-that clauses
 */
class SuchThatStrategy : public EvaluationStrategy {
 private:
  SuchThatClause clause_;

 public:
  SuchThatStrategy(IPKBQuerier *pkb, SuchThatClause &query_clause)
      : EvaluationStrategy(pkb), clause_(query_clause) {};

  void evaluate() override;
};

class PatternStrategy : public EvaluationStrategy {
  PatternClause clause_;

 public:
  PatternStrategy(IPKBQuerier *pkb, PatternClause &query_clause)
      : EvaluationStrategy(pkb), clause_(query_clause) {};

  void evaluate() override;
};

class EvaluationStrategyCreationError : public std::runtime_error {
 public:
  explicit EvaluationStrategyCreationError(const std::string &message) : std::runtime_error(message) {};
};