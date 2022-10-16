#include <gtest/gtest.h>

#include <algorithm>

#include "qps/query_parser/query_builder.h"

TEST(BuilderTest, QueryBuilderTest) {
  auto *query_synonym = new QuerySynonym("v");
  auto *synonym_reference = new SynonymReference(query_synonym, EntityType::kVariable);
  auto *query = new Query({synonym_reference},
                          new SelectCall(new SynonymReference(query_synonym)),
                          {});

  QueryBuilder builder = QueryBuilder(query);
  ASSERT_EQ(builder.build(), query);
}