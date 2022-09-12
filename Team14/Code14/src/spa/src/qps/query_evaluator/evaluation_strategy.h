#pragma once

#include <iterator>
#include <unordered_set>

#include "commons/result.h"
#include "pkb/pkb.h"
#include "qps/exceptions.h"
#include "qps/pql/query_clause.h"

class EvaluationStrategy {
 protected:
  IPKBQuerier *pkb_;

  explicit EvaluationStrategy(IPKBQuerier *pkb) : pkb_(pkb){};

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
  SuchThatStrategy(IPKBQuerier *pkb, SuchThatClause &query_clause) : EvaluationStrategy(pkb), clause_(query_clause){};

  void evaluate() override;
  void intersectContext(QueryDeclaration *param_to_send, QueryDeclaration *param_to_be_intersected, RsType rs_type,
                        bool invert_search);
};

class PatternStrategy : public EvaluationStrategy {
  PatternClause clause_;

 public:
  PatternStrategy(IPKBQuerier *pkb, PatternClause &query_clause) : EvaluationStrategy(pkb), clause_(query_clause){};

  void evaluate() override;
  void intersectContext(QueryDeclaration *assign_param, QueryDeclaration *left_param, QueryDeclaration *right_param);
};
