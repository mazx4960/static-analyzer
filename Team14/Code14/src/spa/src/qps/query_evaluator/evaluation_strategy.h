#pragma once

#include <unordered_set>

#include "pkb/pkb.h"
#include "qps/pql/query_clause/query_clause.h"

class EvaluationStrategy {
 protected:
  PKB pkb_interface_;

  QueryClause query_call_;

  EvaluationStrategy(PKB &pkb_interface, QueryClause &query_clause)
      : pkb_interface_(pkb_interface),
        query_call_(query_clause){};

 public:
  static EvaluationStrategy *getStrategy(PKB &, QueryClause &);
  virtual Result evaluate() = 0;
};

/*
 * Selection for such-that clauses
 */
class SuchThatStrategy : public EvaluationStrategy {
 public:
  SuchThatStrategy(PKB &pkb_interface, QueryClause &query_clause) : EvaluationStrategy(pkb_interface,
                                                                                       query_clause){};
  Result evaluate() override;
};

class PatternStrategy : public EvaluationStrategy {
 public:
  PatternStrategy(PKB &pkb_interface, QueryClause &query_clause) : EvaluationStrategy(pkb_interface,
                                                                                      query_clause){};
  Result evaluate() override;
};