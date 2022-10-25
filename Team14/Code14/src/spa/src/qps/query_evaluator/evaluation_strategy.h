#pragma once

#include <iterator>
#include <unordered_set>

#include "pkb/pkb.h"
#include "qps/exceptions.h"
#include "qps/pql/query_clause.h"
#include "qps/query_evaluator/subquery_result.h"
#include "qps/result.h"
#include "types.h"
#include "context.h"
#include "util.h"

class EvaluationStrategy {
 protected:
  IPKBQuerier *pkb_;

  explicit EvaluationStrategy(IPKBQuerier *pkb) : pkb_(pkb) {
  };

 public:
  /**
   * Factory method to get the correct evaluation strategy for the given query clause.
   * @param pkb PKB interface.
   * @param query_clause query clause.
   * @return pointer to EvaluationStrategy instance.
   */
  static EvaluationStrategy *getStrategy(IPKBQuerier *, QueryClause *);

  virtual SubqueryResult evaluate(Context *ctx) = 0;

  /**
   * Get the candidates for the given query declaration.
   * @param query_declaration query declaration.
   * @return set of Entity candidates.
   */
  EntitySet getCandidates(Context *ctx, QueryReference *);
  static std::string unwrapEntity(QueryReference *, Entity *);
  void updateContext(Context *ctx, QueryReference *synonym_ref, EntitySet &entities);
};

/*
 * Selection for such-that clauses
 */
class SuchThatStrategy : public EvaluationStrategy {
 private:
  SuchThatClause *clause_;

 public:
  SuchThatStrategy(IPKBQuerier *pkb, SuchThatClause *query_clause) : EvaluationStrategy(pkb), clause_(query_clause) {
  };

  /**
   * Evaluate the SuchThat clause.
   * @return true if query clause has results, false otherwise.
   */
  SubqueryResult evaluate(Context *ctx) override;

  /**
   * Evaluate pattern clause given parameters.
   * @param var_param Left-hand side parameter.
   * @param expr_param Right-hand side expression.
   * @return set of Entity pointers matching the parameter and expression.
   */
  EntityPointerUnorderedMap evaluateParameter(Context *ctx, RsType, QueryReference *, QueryReference *);
};

/*
 * Selection for pattern clauses
 */
class PatternStrategy : public EvaluationStrategy {
  PatternClause *clause_;

 public:
  PatternStrategy(IPKBQuerier *pkb, PatternClause *query_clause) : EvaluationStrategy(pkb), clause_(query_clause) {
  };

  /**
  * Evaluate the Pattern clause.
  * @return true if query clause has results, false otherwise.
  */
  SubqueryResult evaluate(Context *ctx) override;

  /**
   * Evaluate query parameter.
   * @param param parameter to evaluated.
   * @param rs_type Relationship type.
   * @param invert_search true if searching by second parameter (e.g. searching by s2 in Follows(s1, s2)).
   * @return set of Entity pointers matching the parameter and relationship type.
   */
  EntityPointerUnorderedMap evaluateParameter(Context *ctx,
                                              SynonymReference *stmt_param,
                                              QueryReference *var_param,
                                              ExpressionSpec *expr_param);
};

/*
 * Selection for pattern clauses
 */
class WithStrategy : public EvaluationStrategy {
  WithClause *clause_;

 public:
  WithStrategy(IPKBQuerier *pkb, WithClause *query_clause) : EvaluationStrategy(pkb), clause_(query_clause) {
  };

  /**
  * Evaluate the With clause.
  * @return true if query clause has results, false otherwise.
  */
  SubqueryResult evaluate(Context *ctx) override;

  /**
   * Evaluate query parameter.
   * @param param parameter to evaluated.
   * @return set of Entity pointers matching the parameter and relationship type.
   */
  EntityPointerUnorderedMap evaluateParameter(Context *ctx, QueryReference *, QueryReference *, Comparator);
};
