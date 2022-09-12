#include <gtest/gtest.h>

#include <algorithm>

#include "qps/query_builder.h"

TEST(BuilderTest, QueryBuilderTest) {
  auto* query_declaration = new VariableDeclaration(QuerySynonym("v"));
  std::vector<QueryDeclaration*> query_declarations = {query_declaration};

  std::vector<QueryClause*> clause_vector;
  QueryCall* query_call = new SelectCall(query_declaration, clause_vector);
  std::vector<QueryCall*> query_calls = {query_call};

  QueryBuilder builder = QueryBuilder();
  builder.withDeclarations(query_declarations);
  builder.withQueryCalls(query_calls);
  Query query = builder.build();

  ASSERT_EQ(query.getDeclarations(), query_declarations);
  ASSERT_EQ(query.getQueryCall().getDeclaration(), query_call->getDeclaration());
}