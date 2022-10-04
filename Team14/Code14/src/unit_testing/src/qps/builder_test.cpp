#include <gtest/gtest.h>

#include <algorithm>

#include "qps/query_parser/query_builder.h"

TEST(BuilderTest, QueryBuilderTest) {
  auto *query_synonym = new QuerySynonym("v");
  auto *query_declaration = new VariableDeclaration(query_synonym);
  std::vector<QueryDeclaration *> query_declarations = {query_declaration};

  std::vector<QueryClause *> clause_vector;
  QueryCall *query_call = new SelectCall(query_declaration, clause_vector);

  QueryBuilder builder = QueryBuilder();
  ASSERT_EQ(*builder.buildDeclaration(EntityType::kVariable, query_synonym), *query_declaration);
}