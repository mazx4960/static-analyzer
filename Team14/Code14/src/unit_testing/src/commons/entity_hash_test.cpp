#include <unordered_set>
#include <vector>

#include "gtest/gtest.h"
#include "commons/entity.h"
/*
 * Equal entity type, same value
 */
TEST(EntityTypeHashTest, EqualProcedureName) {
  auto *e1 = new ProcedureEntity("abc");
  auto *e2 = new ProcedureEntity("abc");
  ASSERT_EQ(EntityHashFunction().operator()(e1), EntityHashFunction().operator()(e2));
  ASSERT_EQ((new std::unordered_set<Entity, EntityHashFunction>({*e1, *e2}))->size(), 1);
}

TEST(EntityTypeHashTest, EqualStatementInt) {
  auto *e1 = new StatementEntity(123);
  auto *e2 = new StatementEntity(123);
  ASSERT_EQ(EntityHashFunction().operator()(e1), EntityHashFunction().operator()(e2));
  ASSERT_EQ((new std::unordered_set<Entity, EntityHashFunction>({*e1, *e2}))->size(), 1);
}

TEST(EntityTypeHashTest, EqualVariableName) {
  auto *e1 = new VariableEntity("abc");
  auto *e2 = new VariableEntity("abc");
  ASSERT_EQ(EntityHashFunction().operator()(e1), EntityHashFunction().operator()(e2));
  ASSERT_EQ((new std::unordered_set<Entity, EntityHashFunction>({*e1, *e2}))->size(), 1);
}

TEST(EntityTypeHashTest, EqualConstantIntStr) {
  auto *e1 = new ConstantEntity("123");
  auto *e2 = new ConstantEntity("123");
  ASSERT_EQ(EntityHashFunction().operator()(e1), EntityHashFunction().operator()(e2));
  ASSERT_EQ((new std::unordered_set<Entity, EntityHashFunction>({*e1, *e2}))->size(), 1);
}

/*
 * Equal entity type, different value
 */
TEST(EntityTypeHashTest, UnequalProcedureName) {
  auto *e1 = new ProcedureEntity("abc");
  auto *e2 = new ProcedureEntity("def");
  ASSERT_NE(EntityHashFunction().operator()(e1), EntityHashFunction().operator()(e2));
  ASSERT_EQ((new std::unordered_set<Entity, EntityHashFunction>({*e1, *e2}))->size(), 2);

}

TEST(EntityTypeHashTest, UnequalStatementInt) {
  auto *e1 = new StatementEntity(123);
  auto *e2 = new StatementEntity(234);
  ASSERT_NE(EntityHashFunction().operator()(e1), EntityHashFunction().operator()(e2));
  ASSERT_EQ((new std::unordered_set<Entity, EntityHashFunction>({*e1, *e2}))->size(), 2);
}

TEST(EntityTypeHashTest, UnequalVariableName) {
  auto *e1 = new VariableEntity("abc");
  auto e2 = new VariableEntity("def");
  ASSERT_NE(EntityHashFunction().operator()(e1), EntityHashFunction().operator()(e2));
  ASSERT_EQ((new std::unordered_set<Entity, EntityHashFunction>({*e1, *e2}))->size(), 2);
}

TEST(EntityTypeHashTest, UnequalConstantIntStr) {
  auto *e1 = new ConstantEntity("123");
  auto *e2 = new ConstantEntity("234");
  ASSERT_NE(EntityHashFunction().operator()(e1), EntityHashFunction().operator()(e2));
  ASSERT_EQ((new std::unordered_set<Entity, EntityHashFunction>({*e1, *e2}))->size(), 2);
}

/*
 * Different entity type, same entity name (integer)
 */
TEST(EntityTypeHashTest, DifferentEntityTypeSameStr) {
  auto *e1 = new ProcedureEntity("123");
  auto *e2 = new VariableEntity("123");
  auto *e3 = new ConstantEntity("123");
  std::unordered_set<size_t> hash_value_set = {
      EntityHashFunction().operator()(e1),
      EntityHashFunction().operator()(e2),
      EntityHashFunction().operator()(e3)
  };
  ASSERT_EQ(hash_value_set.size(), 3); // All distinct
  ASSERT_EQ((new std::unordered_set<Entity, EntityHashFunction>({*e1, *e2, *e3}))->size(), 3);
}

/*
 * Same statement type, same integer
 */
TEST(StatementTypeHashTest, EqualStmt) {
  auto *s1 = new StatementEntity(123);
  auto *s2 = new StatementEntity(123);
  ASSERT_EQ(EntityHashFunction().operator()(s1), EntityHashFunction().operator()(s2));
  ASSERT_EQ((new std::unordered_set<Entity, EntityHashFunction>({*s1, *s2}))->size(), 1);
}
TEST(StatementTypeHashTest, EqualAssign) {
  auto *s1 = new AssignStmtEntity(123);
  auto *s2 = new AssignStmtEntity(123);
  ASSERT_EQ(EntityHashFunction().operator()(s1), EntityHashFunction().operator()(s2));
  ASSERT_EQ((new std::unordered_set<Entity, EntityHashFunction>({*s1, *s2}))->size(), 1);
}
TEST(StatementTypeHashTest, EqualCall) {
  auto *s1 = new CallStmtEntity(123);
  auto *s2 = new CallStmtEntity(123);
  ASSERT_EQ(EntityHashFunction().operator()(s1), EntityHashFunction().operator()(s2));
  ASSERT_EQ((new std::unordered_set<Entity, EntityHashFunction>({*s1, *s2}))->size(), 1);
}
TEST(StatementTypeHashTest, EqualIf) {
  auto *s1 = new IfStmtEntity(123);
  auto *s2 = new IfStmtEntity(123);
  ASSERT_EQ(EntityHashFunction().operator()(s1), EntityHashFunction().operator()(s2));
  ASSERT_EQ((new std::unordered_set<Entity, EntityHashFunction>({*s1, *s2}))->size(), 1);
}
TEST(StatementTypeHashTest, EqualWhile) {
  auto *s1 = new WhileStmtEntity(123);
  auto *s2 = new WhileStmtEntity(123);
  ASSERT_EQ(EntityHashFunction().operator()(s1), EntityHashFunction().operator()(s2));
  ASSERT_EQ((new std::unordered_set<Entity, EntityHashFunction>({*s1, *s2}))->size(), 1);
}
TEST(StatementTypeHashTest, EqualPrint) {
  auto *s1 = new PrintStmtEntity(123);
  auto *s2 = new PrintStmtEntity(123);
  ASSERT_EQ(EntityHashFunction().operator()(s1), EntityHashFunction().operator()(s2));
  ASSERT_EQ((new std::unordered_set<Entity, EntityHashFunction>({*s1, *s2}))->size(), 1);
}
TEST(StatementTypeHashTest, EqualRead) {
  auto *s1 = new ReadStmtEntity(123);
  auto *s2 = new ReadStmtEntity(123);
  ASSERT_EQ(EntityHashFunction().operator()(s1), EntityHashFunction().operator()(s2));
  ASSERT_EQ((new std::unordered_set<Entity, EntityHashFunction>({*s1, *s2}))->size(), 1);
}

/*
 * Same statement type, different integer
 */
TEST(StatementTypeHashTest, UnequalStmt) {
  auto *s1 = new StatementEntity(123);
  auto *s2 = new StatementEntity(234);
  ASSERT_NE(EntityHashFunction().operator()(s1), EntityHashFunction().operator()(s2));
  std::unordered_set<Entity, EntityHashFunction> set = {*s1, *s2};
  ASSERT_EQ(set.size(), 2);
}
TEST(StatementTypeHashTest, UnequalAssign) {
  auto *s1 = new AssignStmtEntity(123);
  auto *s2 = new AssignStmtEntity(234);
  ASSERT_NE(EntityHashFunction().operator()(s1), EntityHashFunction().operator()(s2));
  std::unordered_set<Entity, EntityHashFunction> set = {*s1, *s2};
  ASSERT_EQ(set.size(), 2);
}
TEST(StatementTypeHashTest, UnequalCall) {
  auto *s1 = new CallStmtEntity(123);
  auto *s2 = new CallStmtEntity(234);
  ASSERT_NE(EntityHashFunction().operator()(s1), EntityHashFunction().operator()(s2));
  std::unordered_set<Entity, EntityHashFunction> set = {*s1, *s2};
  ASSERT_EQ(set.size(), 2);
}
TEST(StatementTypeHashTest, UnequalIf) {
  auto *s1 = new IfStmtEntity(123);
  auto *s2 = new IfStmtEntity(234);
  ASSERT_NE(EntityHashFunction().operator()(s1), EntityHashFunction().operator()(s2));
  std::unordered_set<Entity, EntityHashFunction> set = {*s1, *s2};
  ASSERT_EQ(set.size(), 2);
}
TEST(StatementTypeHashTest, UnequalWhile) {
  auto *s1 = new WhileStmtEntity(123);
  auto *s2 = new WhileStmtEntity(234);
  ASSERT_NE(EntityHashFunction().operator()(s1), EntityHashFunction().operator()(s2));
  std::unordered_set<Entity, EntityHashFunction> set = {*s1, *s2};
  ASSERT_EQ(set.size(), 2);
}
TEST(StatementTypeHashTest, UnequalPrint) {
  auto *s1 = new PrintStmtEntity(123);
  auto *s2 = new PrintStmtEntity(234);
  ASSERT_NE(EntityHashFunction().operator()(s1), EntityHashFunction().operator()(s2));
  std::unordered_set<Entity, EntityHashFunction> set = {*s1, *s2};
  ASSERT_EQ(set.size(), 2);
}
TEST(StatementTypeHashTest, UnequalRead) {
  auto *s1 = new ReadStmtEntity(123);
  auto *s2 = new ReadStmtEntity(234);
  ASSERT_NE(EntityHashFunction().operator()(s1), EntityHashFunction().operator()(s2));
  std::unordered_set<Entity, EntityHashFunction> set = {*s1, *s2};
  ASSERT_EQ(set.size(), 2);
}

/*
 * Different statement type, same integer
 */
TEST(StatementTypeHashTest, AssignCall) {
  auto *s1 = new StatementEntity(123);
  auto *s2 = new AssignStmtEntity(123);
  auto *s3 = new CallStmtEntity(123);
  auto *s4 = new IfStmtEntity(123);
  auto *s5 = new WhileStmtEntity(123);
  auto *s6 = new PrintStmtEntity(123);
  auto *s7 = new ReadStmtEntity(123);
  std::unordered_set<size_t> hash_value_set = {
      EntityHashFunction().operator()(s1),
      EntityHashFunction().operator()(s2),
      EntityHashFunction().operator()(s3),
      EntityHashFunction().operator()(s4),
      EntityHashFunction().operator()(s5),
      EntityHashFunction().operator()(s6),
      EntityHashFunction().operator()(s7)
  };
  ASSERT_EQ(hash_value_set.size(), 7); // All distinct
  std::unordered_set<Entity, EntityHashFunction> set = {*s1, *s2, *s3, *s4, *s5, *s6, *s7};
  ASSERT_EQ(set.size(), 7);
}