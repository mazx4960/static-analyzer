//#include <gtest/gtest.h>
//
//#include <algorithm>
//
//#include "qps/query_parser/query_builder.h"
//
//TEST(BuilderTest, QueryBuilderTest) {
//  auto *query_synonym = new QuerySynonym("v");
//  auto *synonym_reference = new SynonymReference(query_synonym, EntityType::kVariable);
//  auto *query = new Query({synonym_reference}, new ElemSelect(new SynonymReference(query_synonym)), {});
//
//  QueryBuilder builder = QueryBuilder();
//  ASSERT_EQ(builder.Build(), query);
//}