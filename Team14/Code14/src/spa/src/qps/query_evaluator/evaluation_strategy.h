#pragma once

#include <iterator>
#include <unordered_set>

#include "pkb/pkb.h"
#include "qps/exceptions.h"
#include "qps/pql/query_clause.h"
#include "qps/query_evaluator/subquery_result.h"
#include "qps/result.h"

using EntitySet = std::unordered_set<Entity *, EntityHashFunction, EntityPointerEquality>;

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

  virtual SubqueryResult evaluate() = 0;

  /**
   * Get the candidates for the given query declaration.
   * @param query_declaration query declaration.
   * @return set of Entity candidates.
   */
  EntitySet getCandidates(QueryReference *);
  static std::string unwrapEntity(QueryReference *, Entity *);

  /**
   * Checks if the context of two QueryDeclarations should be intersected.
   * If the context is empty it means that the QueryDeclaration is a wildcard or a literal.
   * @param declaration query declaration.
   * @return true if the context should be intersected, false otherwise.
   */
  static bool shouldIntersect(QueryReference *);

  /**
   * Intersect the context of two QueryDeclarations.
   * @param first first set of Entity pointers.
   * @param second second set of Entity pointers.
   * @return intersection of sets of Entity pointers.
   */
  static EntitySet intersect(const EntitySet &first, const EntitySet &second);

  /**
   * Copy the elements of a set of Entity pointers that satisfy a predicate.
   * @param candidates
   * @param pred
   * @return
   */
  static EntitySet copy_if(const EntitySet &candidates, const std::function<bool(Entity *)> &pred);
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
  SubqueryResult evaluate() override;

  /**
   * Evaluate pattern clause given parameters.
   * @param var_param Left-hand side parameter.
   * @param expr_param Right-hand side expression.
   * @return set of Entity pointers matching the parameter and expression.
   */
  EntityPointerUnorderedMap evaluateParameter(QueryReference *, RsType, bool, const EntitySet &);
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
  SubqueryResult evaluate() override;

  /**
   * Evaluate query parameter.
   * @param param parameter to evaluated.
   * @param rs_type Relationship type.
   * @param invert_search true if searching by second parameter (e.g. searching by s2 in Follows(s1, s2)).
   * @return set of Entity pointers matching the parameter and relationship type.
   */
  EntityPointerUnorderedMap evaluateParameter(QueryReference *var_param,
                                              ExpressionSpec *expr_param,
                                              const EntitySet &potential_matches);
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
  SubqueryResult evaluate() override;

  /**
   * Evaluate query parameter.
   * @param param parameter to evaluated.
   * @return set of Entity pointers matching the parameter and relationship type.
   */
  EntityPointerUnorderedMap evaluateParameter(QueryReference *, QueryReference *, Comparator);
};
