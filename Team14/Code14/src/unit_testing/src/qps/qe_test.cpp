#include <gtest/gtest.h>

#include "commons/lexer/token.h"
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
  QueryDeclaration query_declaration = QueryDeclaration(test_entity, query_synonym);
  std::vector<QueryDeclaration> query_declarations = {query_declaration};

  Query query = Query(
      query_declarations, QueryCall(CallType::kSelect, QueryDeclaration(test_entity, query_synonym), {})
  );

  QueryEvaluator query_evaluator = QueryEvaluator(pkb, query);
  Result result = query_evaluator.evaluate();
  std::unordered_set<std::string> result_set = result.get_results();
  std::vector<std::string> result_vector(result_set.begin(), result_set.end());
  std::sort(result_vector.begin(), result_vector.end());

  ASSERT_EQ(result_vector, std::vector<std::string>({"x", "y", "z"}));
}