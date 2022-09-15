#include "gtest/gtest.h"

#include "qps/pql/query_synonym.h"

TEST(QuerySynonymPointerEqualityTest, Equal) {
  auto *q1 = new QuerySynonym("s");
  auto *q2 = new QuerySynonym("s");
  ASSERT_EQ((*q1), (*q2));
  ASSERT_EQ(QuerySynonymPointerEquality().operator()(q1, q2), true);
}

TEST(QuerySynonymPointerEqualityTest, Unequal) {
  auto *q1 = new QuerySynonym("s");
  auto *q2 = new QuerySynonym("t");
  ASSERT_NE((*q1), (*q2));
  ASSERT_NE(QuerySynonymPointerEquality().operator()(q1, q2), true);
}

TEST(QuerySynonymObjEqualityTest, Equal) {
  QuerySynonym q1 = QuerySynonym("s");
  QuerySynonym q2 = QuerySynonym("s");
  ASSERT_EQ(q1, q2);
}

TEST(QuerySynonymObjEqualityTest, Unequal) {
  QuerySynonym q1 = QuerySynonym("s");
  QuerySynonym q2 = QuerySynonym("t");
  ASSERT_NE(q1, q2);
}
