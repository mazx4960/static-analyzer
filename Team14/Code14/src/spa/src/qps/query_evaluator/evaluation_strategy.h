#pragma once

#include "pkb/component_interface/pkb_interface.h"
#include "qps/pql/query/query.h"

class EvaluationStrategy {
 protected:
  PKBInterface pkb_interface_;

  Query query_;

  EvaluationStrategy(PKBInterface &pkb_interface, Query &query) : pkb_interface_(pkb_interface), query_(query) {};
 public:
  static EvaluationStrategy *getStrategy(PKBInterface &, Query &);
  virtual Result evaluate() = 0;
};

/*
 * Selection for single type of entity
 * No such-that clauses or pattern clauses
 * e.g. "Select v";
 */
class SelectStrategy : public EvaluationStrategy {
 public:
  SelectStrategy(PKBInterface &pkb_interface, Query &query) : EvaluationStrategy(pkb_interface, query) {};
  Result evaluate() override;
};

/*
 * Selection for such-that clauses
 */
class SuchThatStrategy : public EvaluationStrategy {
 public:
  SuchThatStrategy(PKBInterface &pkb_interface, Query &query) : EvaluationStrategy(pkb_interface, query) {};
  Result evaluate() override;
};
