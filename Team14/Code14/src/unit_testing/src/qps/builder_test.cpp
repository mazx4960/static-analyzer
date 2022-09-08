#include <gtest/gtest.h>

#include <algorithm>
#include "qps/query_parser/query_builder.h"

TEST(BuilderTest, QueryBuilderTest) {
  auto *test_entity =  new VariableEntity("x");
  QuerySynonym query_synonym = QuerySynonym("v");
  auto* query_declaration = new QueryDeclaration(test_entity, query_synonym);
  std::vector<QueryDeclaration*> query_declarations = {query_declaration};

  SuchThatClause such_that_clause = SuchThatClause(ParentRelationship(new VariableEntity("x"), new VariableEntity("y")));
  std::vector<QueryClause> clause_vector = {such_that_clause};
  QueryCall* query_call = new SelectCall(query_declaration, clause_vector);
  std::vector<QueryCall *> query_calls = {query_call};

  QueryBuilder builder = QueryBuilder();
  builder.withDeclarations(query_declarations);
  builder.withQueryCalls(query_calls);
  Query query = builder.build();

  ASSERT_EQ(query.getDeclarations(), query_declarations);
}