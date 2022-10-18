#include <unordered_set>
#include <vector>

#include "commons/entity.h"
#include "gtest/gtest.h"

using EntityPointerUnorderedSet = std::unordered_set<Entity *, EntityHashFunction, EntityPointerEquality>;

/*
 * Equal entity type, same value
 */
TEST(EntityTypeHashTest, EqualProcedureName) {
  auto *e1 = new ProcedureEntity("abc");
  auto *e2 = new ProcedureEntity("abc");
  ASSERT_EQ(EntityHashFunction().operator()(e1), EntityHashFunction().operator()(e2));
  ASSERT_EQ((new EntityPointerUnorderedSet({e1, e2}))->size(), 1);
}

TEST(EntityTypeHashTest, EqualVariableName) {
  auto *e1 = new VariableEntity("abc");
  auto *e2 = new VariableEntity("abc");
  ASSERT_EQ(EntityHashFunction().operator()(e1), EntityHashFunction().operator()(e2));
  ASSERT_EQ((new EntityPointerUnorderedSet({e1, e2}))->size(), 1);
}

TEST(EntityTypeHashTest, EqualConstantIntStr) {
  auto *e1 = new ConstantEntity("123");
  auto *e2 = new ConstantEntity("123");
  ASSERT_EQ(EntityHashFunction().operator()(e1), EntityHashFunction().operator()(e2));
  ASSERT_EQ((new EntityPointerUnorderedSet({e1, e2}))->size(), 1);
}

/*
 * Equal entity type, different value
 */
TEST(EntityTypeHashTest, UnequalProcedureName) {
  auto *e1 = new ProcedureEntity("abc");
  auto *e2 = new ProcedureEntity("def");
  ASSERT_NE(EntityHashFunction().operator()(e1), EntityHashFunction().operator()(e2));
  ASSERT_EQ((new EntityPointerUnorderedSet({e1, e2}))->size(), 2);
}

TEST(EntityTypeHashTest, UnequalVariableName) {
  auto *e1 = new VariableEntity("abc");
  auto *e2 = new VariableEntity("def");
  ASSERT_NE(EntityHashFunction().operator()(e1), EntityHashFunction().operator()(e2));
  ASSERT_EQ((new EntityPointerUnorderedSet({e1, e2}))->size(), 2);
}

TEST(EntityTypeHashTest, UnequalConstantIntStr) {
  auto *e1 = new ConstantEntity("123");
  auto *e2 = new ConstantEntity("234");
  ASSERT_NE(EntityHashFunction().operator()(e1), EntityHashFunction().operator()(e2));
  ASSERT_EQ((new EntityPointerUnorderedSet({e1, e2}))->size(), 2);
}

/*
 * Different entity type, same entity name (integer)
 */
TEST(EntityTypeHashTest, DifferentEntityTypeSameStr) {
  auto *e1 = new ProcedureEntity("123");
  auto *e2 = new VariableEntity("123");
  auto *e3 = new ConstantEntity("123");
  std::unordered_set<size_t> hash_value_set = {EntityHashFunction().operator()(e1), EntityHashFunction().operator()(e2),
                                               EntityHashFunction().operator()(e3)};
  ASSERT_EQ(hash_value_set.size(), 3);// All distinct
  ASSERT_EQ((new EntityPointerUnorderedSet({e1, e2, e3}))->size(), 3);
}

/*
 * Same statement type, same integer
 */
TEST(StatementTypeHashTest, EqualAssign) {
  auto *s1 = new AssignStmtEntity("123");
  auto *s2 = new AssignStmtEntity("123");
  ASSERT_EQ(EntityHashFunction().operator()(s1), EntityHashFunction().operator()(s2));
  ASSERT_EQ((new EntityPointerUnorderedSet({s1, s2}))->size(), 1);
}
TEST(StatementTypeHashTest, EqualCall) {
  auto *s1 = new CallStmtEntity("123", std::string());
  auto *s2 = new CallStmtEntity("123", std::string());
  ASSERT_EQ(EntityHashFunction().operator()(s1), EntityHashFunction().operator()(s2));
  ASSERT_EQ((new EntityPointerUnorderedSet({s1, s2}))->size(), 1);
}
TEST(StatementTypeHashTest, EqualIf) {
  auto *s1 = new IfStmtEntity("123");
  auto *s2 = new IfStmtEntity("123");
  ASSERT_EQ(EntityHashFunction().operator()(s1), EntityHashFunction().operator()(s2));
  ASSERT_EQ((new EntityPointerUnorderedSet({s1, s2}))->size(), 1);
}
TEST(StatementTypeHashTest, EqualWhile) {
  auto *s1 = new WhileStmtEntity("123");
  auto *s2 = new WhileStmtEntity("123");
  ASSERT_EQ(EntityHashFunction().operator()(s1), EntityHashFunction().operator()(s2));
  ASSERT_EQ((new EntityPointerUnorderedSet({s1, s2}))->size(), 1);
}
TEST(StatementTypeHashTest, EqualPrint) {
  auto *s1 = new PrintStmtEntity("123", std::string());
  auto *s2 = new PrintStmtEntity("123", std::string());
  ASSERT_EQ(EntityHashFunction().operator()(s1), EntityHashFunction().operator()(s2));
  ASSERT_EQ((new EntityPointerUnorderedSet({s1, s2}))->size(), 1);
}
TEST(StatementTypeHashTest, EqualRead) {
  auto *s1 = new ReadStmtEntity("123", std::string());
  auto *s2 = new ReadStmtEntity("123", std::string());
  ASSERT_EQ(EntityHashFunction().operator()(s1), EntityHashFunction().operator()(s2));
  ASSERT_EQ((new EntityPointerUnorderedSet({s1, s2}))->size(), 1);
}

/*
 * Same statement type, different integer
 */
TEST(StatementTypeHashTest, UnequalAssign) {
  auto *s1 = new AssignStmtEntity("123");
  auto *s2 = new AssignStmtEntity("234");
  ASSERT_NE(EntityHashFunction().operator()(s1), EntityHashFunction().operator()(s2));
  EntityPointerUnorderedSet set = {s1, s2};
  ASSERT_EQ(set.size(), 2);
}
TEST(StatementTypeHashTest, UnequalCall) {
  auto *s1 = new CallStmtEntity("123", std::string());
  auto *s2 = new CallStmtEntity("234", std::string());
  ASSERT_NE(EntityHashFunction().operator()(s1), EntityHashFunction().operator()(s2));
  EntityPointerUnorderedSet set = {s1, s2};
  ASSERT_EQ(set.size(), 2);
}
TEST(StatementTypeHashTest, UnequalIf) {
  auto *s1 = new IfStmtEntity("123");
  auto *s2 = new IfStmtEntity("234");
  ASSERT_NE(EntityHashFunction().operator()(s1), EntityHashFunction().operator()(s2));
  EntityPointerUnorderedSet set = {s1, s2};
  ASSERT_EQ(set.size(), 2);
}
TEST(StatementTypeHashTest, UnequalWhile) {
  auto *s1 = new WhileStmtEntity("123");
  auto *s2 = new WhileStmtEntity("234");
  ASSERT_NE(EntityHashFunction().operator()(s1), EntityHashFunction().operator()(s2));
  EntityPointerUnorderedSet set = {s1, s2};
  ASSERT_EQ(set.size(), 2);
}
TEST(StatementTypeHashTest, UnequalPrint) {
  auto *s1 = new PrintStmtEntity("123", std::string());
  auto *s2 = new PrintStmtEntity("234", std::string());
  ASSERT_NE(EntityHashFunction().operator()(s1), EntityHashFunction().operator()(s2));
  EntityPointerUnorderedSet set = {s1, s2};
  ASSERT_EQ(set.size(), 2);
}
TEST(StatementTypeHashTest, UnequalRead) {
  auto *s1 = new ReadStmtEntity("123", std::string());
  auto *s2 = new ReadStmtEntity("234", std::string());
  ASSERT_NE(EntityHashFunction().operator()(s1), EntityHashFunction().operator()(s2));
  EntityPointerUnorderedSet set = {s1, s2};
  ASSERT_EQ(set.size(), 2);
}

/*
 * Different entity type, same value
 */
TEST(EntityTypeHashTest, DiffPointerSameEntity) {
  auto *s1 = new VariableEntity("123");
  auto *s2 = new VariableEntity("123");
  auto *s3 = new VariableEntity("123");
  auto *s4 = new VariableEntity("123");
  auto *s5 = new VariableEntity("123");
  auto *s6 = new VariableEntity("123");
  EntityPointerUnorderedSet set = {s1, s2, s3, s4, s5, s6};
  ASSERT_EQ(set.size(), 1);
}