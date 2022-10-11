#include "gtest/gtest.h"
#include "qps/pql/query_reference.h"
#include "qps/pql/expression_spec.h"

TEST(SynonymReferencePointerEqualityTest, EqualStatement) {
  auto *q1 = new StatementDeclaration(new QuerySynonym("s"));
  auto *q2 = new StatementDeclaration(new QuerySynonym("s"));
  std::unordered_set<SynonymReference *, SynonymReferenceHashFunction, SynonymReferencePointerEquality> s = {q1, q2};
  ASSERT_EQ(s.size(), 1);
  ASSERT_TRUE((*q1) == (*q2));
}
TEST(SynonymReferencePointerEqualityTest, EqualRead) {
  auto *q1 = new ReadDeclaration(new QuerySynonym("s"));
  auto *q2 = new ReadDeclaration(new QuerySynonym("s"));
  std::unordered_set<SynonymReference *, SynonymReferenceHashFunction, SynonymReferencePointerEquality> s = {q1, q2};
  ASSERT_EQ(s.size(), 1);
  ASSERT_TRUE((*q1) == (*q2));
}
TEST(SynonymReferencePointerEqualityTest, EqualCall) {
  auto *q1 = new CallDeclaration(new QuerySynonym("s"));
  auto *q2 = new CallDeclaration(new QuerySynonym("s"));
  std::unordered_set<SynonymReference *, SynonymReferenceHashFunction, SynonymReferencePointerEquality> s = {q1, q2};
  ASSERT_EQ(s.size(), 1);
  ASSERT_TRUE((*q1) == (*q2));
}
TEST(SynonymReferencePointerEqualityTest, EqualPrint) {
  auto *q1 = new PrintDeclaration(new QuerySynonym("s"));
  auto *q2 = new PrintDeclaration(new QuerySynonym("s"));
  std::unordered_set<SynonymReference *, SynonymReferenceHashFunction, SynonymReferencePointerEquality> s = {q1, q2};
  ASSERT_EQ(s.size(), 1);
  ASSERT_TRUE((*q1) == (*q2));
}
TEST(SynonymReferencePointerEqualityTest, EqualWhile) {
  auto *q1 = new WhileDeclaration(new QuerySynonym("s"));
  auto *q2 = new WhileDeclaration(new QuerySynonym("s"));
  std::unordered_set<SynonymReference *, SynonymReferenceHashFunction, SynonymReferencePointerEquality> s = {q1, q2};
  ASSERT_EQ(s.size(), 1);
  ASSERT_TRUE((*q1) == (*q2));
}
TEST(SynonymReferencePointerEqualityTest, EqualIf) {
  auto *q1 = new IfDeclaration(new QuerySynonym("s"));
  auto *q2 = new IfDeclaration(new QuerySynonym("s"));
  std::unordered_set<SynonymReference *, SynonymReferenceHashFunction, SynonymReferencePointerEquality> s = {q1, q2};
  ASSERT_EQ(s.size(), 1);
  ASSERT_TRUE((*q1) == (*q2));
}
TEST(SynonymReferencePointerEqualityTest, EqualAssign) {
  auto *q1 = new AssignDeclaration(new QuerySynonym("s"));
  auto *q2 = new AssignDeclaration(new QuerySynonym("s"));
  std::unordered_set<SynonymReference *, SynonymReferenceHashFunction, SynonymReferencePointerEquality> s = {q1, q2};
  ASSERT_EQ(s.size(), 1);
  ASSERT_TRUE((*q1) == (*q2));
}
TEST(SynonymReferencePointerEqualityTest, EqualVariable) {
  auto *q1 = new VariableDeclaration(new QuerySynonym("s"));
  auto *q2 = new VariableDeclaration(new QuerySynonym("s"));
  std::unordered_set<SynonymReference *, SynonymReferenceHashFunction, SynonymReferencePointerEquality> s = {q1, q2};
  ASSERT_EQ(s.size(), 1);
  ASSERT_TRUE((*q1) == (*q2));
}
TEST(SynonymReferencePointerEqualityTest, EqualConstant) {
  auto *q1 = new ConstantDeclaration(new QuerySynonym("s"));
  auto *q2 = new ConstantDeclaration(new QuerySynonym("s"));
  std::unordered_set<SynonymReference *, SynonymReferenceHashFunction, SynonymReferencePointerEquality> s = {q1, q2};
  ASSERT_EQ(s.size(), 1);
  ASSERT_TRUE((*q1) == (*q2));
}
TEST(SynonymReferencePointerEqualityTest, EqualProcedure) {
  auto *q1 = new ProcedureDeclaration(new QuerySynonym("s"));
  auto *q2 = new ProcedureDeclaration(new QuerySynonym("s"));
  std::unordered_set<SynonymReference *, SynonymReferenceHashFunction, SynonymReferencePointerEquality> s = {q1, q2};
  ASSERT_EQ(s.size(), 1);
  ASSERT_TRUE((*q1) == (*q2));
}
