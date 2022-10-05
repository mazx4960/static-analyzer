#include <gtest/gtest.h>

#include <algorithm>

#include "qps/query_parser/query_builder.h"

TEST(BuilderTest, QueryBuilderTest) {
  auto *query_synonym = new QuerySynonym("v");
  auto *query_declaration = new SynonymDeclaration(query_synonym, EntityType::kVariable);
  std::vector<SynonymDeclaration *> query_declarations = {query_declaration};

  std::vector<QueryClause *> clause_vector;
  QueryCall *query_call = new SelectCall(query_declaration, clause_vector);

  QueryBuilder builder = QueryBuilder();
  ASSERT_EQ(*builder.buildDeclaration(new SynonymDeclaration(query_synonym, EntityType::kVariable)), *query_declaration);
}