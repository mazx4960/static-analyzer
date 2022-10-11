#include <gtest/gtest.h>

#include <algorithm>

#include "qps/query_parser/query_builder.h"

TEST(BuilderTest, QueryBuilderTest) {
  auto *query_synonym = new QuerySynonym("v");
  auto *query_declaration = new SynonymReference(query_synonym, EntityType::kVariable);
  std::vector<SynonymReference *> query_declarations = {query_declaration};

  QueryBuilder builder = QueryBuilder();
  ASSERT_EQ(*builder.buildDeclaration(new SynonymReference(query_synonym, EntityType::kVariable)), *query_declaration);
}