#include "gtest/gtest.h"
#include "qps/pql/query_declaration.h"

TEST(QueryDeclarationPointerEqualityTest, EqualStatement) {
  auto *q1 = new StatementDeclaration(new QuerySynonym("s"));
  auto *q2 = new StatementDeclaration(new QuerySynonym("s"));
  std::unordered_set<QueryDeclaration *, QueryDeclarationHashFunction, QueryDeclarationPointerEquality> s = {q1, q2};
  ASSERT_EQ(s.size(), 1);
  ASSERT_TRUE((*q1) == (*q2));
}
TEST(QueryDeclarationPointerEqualityTest, EqualRead) {
  auto *q1 = new ReadDeclaration(new QuerySynonym("s"));
  auto *q2 = new ReadDeclaration(new QuerySynonym("s"));
  std::unordered_set<QueryDeclaration *, QueryDeclarationHashFunction, QueryDeclarationPointerEquality> s = {q1, q2};
  ASSERT_EQ(s.size(), 1);
  ASSERT_TRUE((*q1) == (*q2));
}
TEST(QueryDeclarationPointerEqualityTest, EqualCall) {
  auto *q1 = new CallDeclaration(new QuerySynonym("s"));
  auto *q2 = new CallDeclaration(new QuerySynonym("s"));
  std::unordered_set<QueryDeclaration *, QueryDeclarationHashFunction, QueryDeclarationPointerEquality> s = {q1, q2};
  ASSERT_EQ(s.size(), 1);
  ASSERT_TRUE((*q1) == (*q2));
}
TEST(QueryDeclarationPointerEqualityTest, EqualPrint) {
  auto *q1 = new PrintDeclaration(new QuerySynonym("s"));
  auto *q2 = new PrintDeclaration(new QuerySynonym("s"));
  std::unordered_set<QueryDeclaration *, QueryDeclarationHashFunction, QueryDeclarationPointerEquality> s = {q1, q2};
  ASSERT_EQ(s.size(), 1);
  ASSERT_TRUE((*q1) == (*q2));
}
TEST(QueryDeclarationPointerEqualityTest, EqualWhile) {
  auto *q1 = new WhileDeclaration(new QuerySynonym("s"));
  auto *q2 = new WhileDeclaration(new QuerySynonym("s"));
  std::unordered_set<QueryDeclaration *, QueryDeclarationHashFunction, QueryDeclarationPointerEquality> s = {q1, q2};
  ASSERT_EQ(s.size(), 1);
  ASSERT_TRUE((*q1) == (*q2));
}
TEST(QueryDeclarationPointerEqualityTest, EqualIf) {
  auto *q1 = new IfDeclaration(new QuerySynonym("s"));
  auto *q2 = new IfDeclaration(new QuerySynonym("s"));
  std::unordered_set<QueryDeclaration *, QueryDeclarationHashFunction, QueryDeclarationPointerEquality> s = {q1, q2};
  ASSERT_EQ(s.size(), 1);
  ASSERT_TRUE((*q1) == (*q2));
}
TEST(QueryDeclarationPointerEqualityTest, EqualAssign) {
  auto *q1 = new AssignDeclaration(new QuerySynonym("s"));
  auto *q2 = new AssignDeclaration(new QuerySynonym("s"));
  std::unordered_set<QueryDeclaration *, QueryDeclarationHashFunction, QueryDeclarationPointerEquality> s = {q1, q2};
  ASSERT_EQ(s.size(), 1);
  ASSERT_TRUE((*q1) == (*q2));
}
TEST(QueryDeclarationPointerEqualityTest, EqualVariable) {
  auto *q1 = new VariableDeclaration(new QuerySynonym("s"));
  auto *q2 = new VariableDeclaration(new QuerySynonym("s"));
  std::unordered_set<QueryDeclaration *, QueryDeclarationHashFunction, QueryDeclarationPointerEquality> s = {q1, q2};
  ASSERT_EQ(s.size(), 1);
  ASSERT_TRUE((*q1) == (*q2));
}
TEST(QueryDeclarationPointerEqualityTest, EqualConstant) {
  auto *q1 = new ConstantDeclaration(new QuerySynonym("s"));
  auto *q2 = new ConstantDeclaration(new QuerySynonym("s"));
  std::unordered_set<QueryDeclaration *, QueryDeclarationHashFunction, QueryDeclarationPointerEquality> s = {q1, q2};
  ASSERT_EQ(s.size(), 1);
  ASSERT_TRUE((*q1) == (*q2));
}
TEST(QueryDeclarationPointerEqualityTest, EqualProcedure) {
  auto *q1 = new ProcedureDeclaration(new QuerySynonym("s"));
  auto *q2 = new ProcedureDeclaration(new QuerySynonym("s"));
  std::unordered_set<QueryDeclaration *, QueryDeclarationHashFunction, QueryDeclarationPointerEquality> s = {q1, q2};
  ASSERT_EQ(s.size(), 1);
  ASSERT_TRUE((*q1) == (*q2));
}
TEST(QueryDeclarationPointerEqualityTest, EqualWildCard) {
  auto *q1 = new WildcardDeclaration();
  auto *q2 = new WildcardDeclaration();
  std::unordered_set<QueryDeclaration *, QueryDeclarationHashFunction, QueryDeclarationPointerEquality> s = {q1, q2};
  ASSERT_EQ(s.size(), 1);
  ASSERT_TRUE((*q1) == (*q2));
}
TEST(QueryDeclarationPointerEqualityTest, EqualExpression) {
  auto *q1 = new ExpressionDeclaration("x + y * z");
  auto *q2 = new ExpressionDeclaration("x + y * z");
  std::unordered_set<QueryDeclaration *, QueryDeclarationHashFunction, QueryDeclarationPointerEquality> s = {q1, q2};
  ASSERT_EQ(s.size(), 1);
  ASSERT_TRUE((*q1) == (*q2));
}
TEST(QueryDeclarationPointerEqualityTest, EqualString) {
  auto *q1 = new IdentDeclaration("str");
  auto *q2 = new IdentDeclaration("str");
  std::unordered_set<QueryDeclaration *, QueryDeclarationHashFunction, QueryDeclarationPointerEquality> s = {q1, q2};
  ASSERT_EQ(s.size(), 1);
  ASSERT_TRUE((*q1) == (*q2));
}
TEST(QueryDeclarationPointerEqualityTest, EqualInteger) {
  auto *q1 = new IntegerDeclaration("123");
  auto *q2 = new IntegerDeclaration("123");
  std::unordered_set<QueryDeclaration *, QueryDeclarationHashFunction, QueryDeclarationPointerEquality> s = {q1, q2};
  ASSERT_EQ(s.size(), 1);
  ASSERT_TRUE((*q1) == (*q2));
}