#pragma once

#include <unordered_set>
#include "pkb/component_interface/pkb_interface.h"
#include "qps/pql/query/query.h"

class EvaluationStrategy {
 protected:
  PKBInterface pkb_interface_;

  QueryCall query_call_;

  EvaluationStrategy(PKBInterface &pkb_interface, QueryCall &query_call)
      : pkb_interface_(pkb_interface), query_call_(query_call) {};
 public:
  static EvaluationStrategy *getStrategy(PKBInterface &, QueryCall &);
  virtual Result evaluate() = 0;
};

/*
 * Selection for such-that clauses
 */
class SuchThatStrategy : public EvaluationStrategy {
 public:
  SuchThatStrategy(PKBInterface &pkb_interface, QueryCall &query_call) : EvaluationStrategy(pkb_interface,
                                                                                            query_call) {};
  Result evaluate() override;
};
