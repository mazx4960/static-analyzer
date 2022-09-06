#include <gtest/gtest.h>

#include <algorithm>
#include "pkb/pkb.h"
#include "qps/query_evaluator/query_evaluator.h"

TEST(QETest, VariableGetTest) {
  PKB *pkb = new PKB();
  std::vector<Entity *> entities = {
      new VariableEntity("x"),
      new VariableEntity("y"),
      new VariableEntity("z")
  };
  pkb->save(entities);
  VariableEntity test_entity = VariableEntity("x");
  QuerySynonym query_synonym = QuerySynonym("v");

  auto *query_declaration = new QueryDeclaration(test_entity, query_synonym);
  std::vector<QueryDeclaration *> query_declarations = {query_declaration};

  std::vector<QueryClause> query_clauses;
  Query query = Query(
      query_declarations, SelectCall(query_declaration, query_clauses)
  );

  QueryEvaluator query_evaluator = QueryEvaluator(pkb, query);
  Result result = query_evaluator.evaluate();
  std::vector<std::string> results_vector = result.get_sorted_results_list();

  ASSERT_EQ(results_vector, std::vector<std::string>({"x", "y", "z"}));
}