#include <unordered_set>

#include "gtest/gtest.h"
#include "commons/entity.h"
/*
 * Equal entity type, same entity name (int)
 */
TEST(EntityTypeHashTest, EqualProcedureInt) {
  auto *e1 = new Entity(EntityType::kProcedure, "123");
  auto *e2 = new Entity(EntityType::kProcedure, "123");
  ASSERT_EQ(EntityHashFunction().operator()(e1), EntityHashFunction().operator()(e2));
}

TEST(EntityTypeHashTest, EqualStatementInt) {
  auto *e1 = new Entity(EntityType::kStatement, "123");
  auto *e2 = new Entity(EntityType::kStatement, "123");
  ASSERT_EQ(EntityHashFunction().operator()(e1), EntityHashFunction().operator()(e2));
}

TEST(EntityTypeHashTest, EqualVariableInt) {
  auto *e1 = new Entity(EntityType::kVariable, "123");
  auto *e2 = new Entity(EntityType::kVariable, "123");
  ASSERT_EQ(EntityHashFunction().operator()(e1), EntityHashFunction().operator()(e2));
}

TEST(EntityTypeHashTest, EqualConstantInt) {
  auto *e1 = new Entity(EntityType::kConstant, "123");
  auto *e2 = new Entity(EntityType::kConstant, "123");
  ASSERT_EQ(EntityHashFunction().operator()(e1), EntityHashFunction().operator()(e2));
}

/*
 * Equal entity type, same entity name (characters)
 */
TEST(EntityTypeHashTest, EqualProcedureStr) {
  auto *e1 = new Entity(EntityType::kProcedure, "abc");
  auto *e2 = new Entity(EntityType::kProcedure, "abc");
  ASSERT_EQ(EntityHashFunction().operator()(e1), EntityHashFunction().operator()(e2));
}

TEST(EntityTypeHashTest, EqualStatementStr) {
  auto *e1 = new Entity(EntityType::kStatement, "abc");
  auto *e2 = new Entity(EntityType::kStatement, "abc");
  ASSERT_EQ(EntityHashFunction().operator()(e1), EntityHashFunction().operator()(e2));
}

TEST(EntityTypeHashTest, EqualVariableStr) {
  auto *e1 = new Entity(EntityType::kVariable, "abc");
  auto *e2 = new Entity(EntityType::kVariable, "abc");
  ASSERT_EQ(EntityHashFunction().operator()(e1), EntityHashFunction().operator()(e2));
}

TEST(EntityTypeHashTest, EqualConstantStr) {
  auto *e1 = new Entity(EntityType::kConstant, "abc");
  auto *e2 = new Entity(EntityType::kConstant, "abc");
  ASSERT_EQ(EntityHashFunction().operator()(e1), EntityHashFunction().operator()(e2));
}

/*
 * Equal entity type, different entity name (integer)
 */
TEST(EntityTypeHashTest, UnequalProcedureInt) {
  auto *e1 = new Entity(EntityType::kProcedure, "123");
  auto *e2 = new Entity(EntityType::kProcedure, "234");
  ASSERT_NE(EntityHashFunction().operator()(e1), EntityHashFunction().operator()(e2));
}

TEST(EntityTypeHashTest, UnequalStatementInt) {
  auto *e1 = new Entity(EntityType::kStatement, "123");
  auto *e2 = new Entity(EntityType::kStatement, "234");
  ASSERT_NE(EntityHashFunction().operator()(e1), EntityHashFunction().operator()(e2));
}

TEST(EntityTypeHashTest, UnequalVariableInt) {
  auto *e1 = new Entity(EntityType::kVariable, "123");
  auto *e2 = new Entity(EntityType::kVariable, "234");
  ASSERT_NE(EntityHashFunction().operator()(e1), EntityHashFunction().operator()(e2));
}

TEST(EntityTypeHashTest, UnequalConstantInt) {
  auto *e1 = new Entity(EntityType::kConstant, "123");
  auto *e2 = new Entity(EntityType::kConstant, "234");
  ASSERT_NE(EntityHashFunction().operator()(e1), EntityHashFunction().operator()(e2));
}

/*
 * Equal entity type, different entity name (characters)
 */
TEST(EntityTypeHashTest, UnequalProcedureStr) {
  auto *e1 = new Entity(EntityType::kProcedure, "abc");
  auto *e2 = new Entity(EntityType::kProcedure, "def");
  ASSERT_NE(EntityHashFunction().operator()(e1), EntityHashFunction().operator()(e2));
}

TEST(EntityTypeHashTest, UnequalStatementStr) {
  auto *e1 = new Entity(EntityType::kStatement, "abc");
  auto *e2 = new Entity(EntityType::kStatement, "def");
  ASSERT_NE(EntityHashFunction().operator()(e1), EntityHashFunction().operator()(e2));
}

TEST(EntityTypeHashTest, UnequalVariableStr) {
  auto *e1 = new Entity(EntityType::kVariable, "abc");
  auto *e2 = new Entity(EntityType::kVariable, "def");
  ASSERT_NE(EntityHashFunction().operator()(e1), EntityHashFunction().operator()(e2));
}

TEST(EntityTypeHashTest, UnequalConstantStr) {
  auto *e1 = new Entity(EntityType::kConstant, "abc");
  auto *e2 = new Entity(EntityType::kConstant, "def");
  ASSERT_NE(EntityHashFunction().operator()(e1), EntityHashFunction().operator()(e2));
}

/*
 * Different entity type, same entity name (integer)
 */
TEST(EntityTypeHashTest, DifferentEntityTypeSameInt) {
  auto *e1 = new Entity(EntityType::kProcedure, "123");
  auto *e2 = new Entity(EntityType::kStatement, "123");
  auto *e3 = new Entity(EntityType::kVariable, "123");
  auto *e4 = new Entity(EntityType::kConstant, "123");
  std::unordered_set<size_t> set = {
      EntityHashFunction().operator()(e1),
      EntityHashFunction().operator()(e2),
      EntityHashFunction().operator()(e3),
      EntityHashFunction().operator()(e4),
  };
  ASSERT_EQ(set.size(), 1); // All distinct
}

/*
 * Different entity type, same entity name (character)
 */
TEST(EntityTypeHashTest, DifferentEntitySameStr) {
  auto *e1 = new Entity(EntityType::kProcedure, "abc");
  auto *e2 = new Entity(EntityType::kStatement, "abc");
  auto *e3 = new Entity(EntityType::kVariable, "abc");
  auto *e4 = new Entity(EntityType::kConstant, "abc");
  std::unordered_set<size_t> set = {
      EntityHashFunction().operator()(e1),
      EntityHashFunction().operator()(e2),
      EntityHashFunction().operator()(e3),
      EntityHashFunction().operator()(e4),
  };
  ASSERT_EQ(set.size(), 1); // All equal
}

/*
 * Same statement type, same integer
 */
TEST(StatementTypeHashTest, EqualAssign) {
  auto *s1 = new StatementEntity(123);
  auto *s2 = new StatementEntity(123);
  ASSERT_EQ(EntityHashFunction().operator()(s1), EntityHashFunction().operator()(s2));
}
TEST(StatementTypeHashTest, EqualCall) {
  auto *s1 = new StatementEntity(123);
  auto *s2 = new StatementEntity(123);
  ASSERT_EQ(EntityHashFunction().operator()(s1), EntityHashFunction().operator()(s2));
}
TEST(StatementTypeHashTest, EqualIf) {
  auto *s1 = new StatementEntity(123);
  auto *s2 = new StatementEntity(123);
  ASSERT_EQ(EntityHashFunction().operator()(s1), EntityHashFunction().operator()(s2));
}
TEST(StatementTypeHashTest, EqualWhile) {
  auto *s1 = new StatementEntity(123);
  auto *s2 = new StatementEntity(123);
  ASSERT_EQ(EntityHashFunction().operator()(s1), EntityHashFunction().operator()(s2));
}
TEST(StatementTypeHashTest, EqualPrint) {
  auto *s1 = new StatementEntity(123);
  auto *s2 = new StatementEntity(123);
  ASSERT_EQ(EntityHashFunction().operator()(s1), EntityHashFunction().operator()(s2));
}
TEST(StatementTypeHashTest, EqualRead) {
  auto *s1 = new StatementEntity(123);
  auto *s2 = new StatementEntity(123);
  ASSERT_EQ(EntityHashFunction().operator()(s1), EntityHashFunction().operator()(s2));
}

/*
 * Same statement type, different integer
 */
TEST(StatementTypeHashTest, UnequalAssign) {
  auto *s1 = new StatementEntity(123);
  auto *s2 = new StatementEntity(234);
  ASSERT_NE(EntityHashFunction().operator()(s1), EntityHashFunction().operator()(s2));
}
TEST(StatementTypeHashTest, UnequalCall) {
  auto *s1 = new StatementEntity(123);
  auto *s2 = new StatementEntity(234);
  ASSERT_NE(EntityHashFunction().operator()(s1), EntityHashFunction().operator()(s2));
}
TEST(StatementTypeHashTest, UnequalIf) {
  auto *s1 = new StatementEntity(123);
  auto *s2 = new StatementEntity(234);
  ASSERT_NE(EntityHashFunction().operator()(s1), EntityHashFunction().operator()(s2));
}
TEST(StatementTypeHashTest, UnequalWhile) {
  auto *s1 = new StatementEntity(123);
  auto *s2 = new StatementEntity(234);
  ASSERT_NE(EntityHashFunction().operator()(s1), EntityHashFunction().operator()(s2));
}
TEST(StatementTypeHashTest, UnequalPrint) {
  auto *s1 = new StatementEntity(123);
  auto *s2 = new StatementEntity(234);
  ASSERT_NE(EntityHashFunction().operator()(s1), EntityHashFunction().operator()(s2));
}
TEST(StatementTypeHashTest, UnequalRead) {
  auto *s1 = new StatementEntity(123);
  auto *s2 = new StatementEntity(234);
  ASSERT_NE(EntityHashFunction().operator()(s1), EntityHashFunction().operator()(s2));
}

/*
 * Different statement type, same integer
 */
TEST(StatementTypeHashTest, AssignCall) {
  auto *s1 = new StatementEntity(123);
  auto *s2 = new StatementEntity(123);
  auto *s3 = new StatementEntity(123);
  auto *s4 = new StatementEntity(123);
  auto *s5 = new StatementEntity(123);
  auto *s6 = new StatementEntity(123);
  std::unordered_set<size_t> set = {
      EntityHashFunction().operator()(s1),
      EntityHashFunction().operator()(s2),
      EntityHashFunction().operator()(s3),
      EntityHashFunction().operator()(s4),
      EntityHashFunction().operator()(s5),
      EntityHashFunction().operator()(s6),
  };
  ASSERT_EQ(set.size(), 1); // All distinct
}

/*
 * Entity type vs Statement type,
 */
TEST(CrossTypeHashTest, ProcedureAssign) {
  auto *e1 = new Entity(EntityType::kProcedure, "123");
  auto *s1 = new StatementEntity(123);
  ASSERT_NE(EntityHashFunction().operator()(e1), EntityHashFunction().operator()(s1));
}
TEST(CrossTypeHashTest, ProcedureCall) {
  auto *e1 = new Entity(EntityType::kProcedure, "123");
  auto *s1 = new StatementEntity(123);
  ASSERT_NE(EntityHashFunction().operator()(e1), EntityHashFunction().operator()(s1));
}
TEST(CrossTypeHashTest, ProcedureIf) {
  auto *e1 = new Entity(EntityType::kProcedure, "123");
  auto *s1 = new StatementEntity(123);
  ASSERT_NE(EntityHashFunction().operator()(e1), EntityHashFunction().operator()(s1));
}
TEST(CrossTypeHashTest, ProcedureWhile) {
  auto *e1 = new Entity(EntityType::kProcedure, "123");
  auto *s1 = new StatementEntity(123);
  ASSERT_NE(EntityHashFunction().operator()(e1), EntityHashFunction().operator()(s1));
}
TEST(CrossTypeHashTest, ProcedurePrint) {
  auto *e1 = new Entity(EntityType::kProcedure, "123");
  auto *s1 = new StatementEntity(123);
  ASSERT_NE(EntityHashFunction().operator()(e1), EntityHashFunction().operator()(s1));
}
TEST(CrossTypeHashTest, ProcedureRead) {
  auto *e1 = new Entity(EntityType::kProcedure, "123");
  auto *s1 = new StatementEntity(123);
  ASSERT_NE(EntityHashFunction().operator()(e1), EntityHashFunction().operator()(s1));
}
TEST(CrossTypeHashTest, StatementAssign) {
  auto *e1 = new Entity(EntityType::kStatement, "123");
  auto *s1 = new StatementEntity(123);
  ASSERT_NE(EntityHashFunction().operator()(e1), EntityHashFunction().operator()(s1));
}
TEST(CrossTypeHashTest, StatementCall) {
  auto *e1 = new Entity(EntityType::kStatement, "123");
  auto *s1 = new StatementEntity(123);
  ASSERT_NE(EntityHashFunction().operator()(e1), EntityHashFunction().operator()(s1));
}
TEST(CrossTypeHashTest, StatementIf) {
  auto *e1 = new Entity(EntityType::kStatement, "123");
  auto *s1 = new StatementEntity(123);
  ASSERT_NE(EntityHashFunction().operator()(e1), EntityHashFunction().operator()(s1));
}
TEST(CrossTypeHashTest, StatementWhile) {
  auto *e1 = new Entity(EntityType::kStatement, "123");
  auto *s1 = new StatementEntity(123);
  ASSERT_NE(EntityHashFunction().operator()(e1), EntityHashFunction().operator()(s1));
}
TEST(CrossTypeHashTest, StatementPrint) {
  auto *e1 = new Entity(EntityType::kStatement, "123");
  auto *s1 = new StatementEntity(123);
  ASSERT_NE(EntityHashFunction().operator()(e1), EntityHashFunction().operator()(s1));
}
TEST(CrossTypeHashTest, StatementRead) {
  auto *e1 = new Entity(EntityType::kStatement, "123");
  auto *s1 = new StatementEntity(123);
  ASSERT_NE(EntityHashFunction().operator()(e1), EntityHashFunction().operator()(s1));
}
TEST(CrossTypeHashTest, VariableAssign) {
  auto *e1 = new Entity(EntityType::kVariable, "123");
  auto *s1 = new StatementEntity(123);
  ASSERT_NE(EntityHashFunction().operator()(e1), EntityHashFunction().operator()(s1));
}
TEST(CrossTypeHashTest, VariableCall) {
  auto *e1 = new Entity(EntityType::kVariable, "123");
  auto *s1 = new StatementEntity(123);
  ASSERT_NE(EntityHashFunction().operator()(e1), EntityHashFunction().operator()(s1));
}
TEST(CrossTypeHashTest, VariableIf) {
  auto *e1 = new Entity(EntityType::kVariable, "123");
  auto *s1 = new StatementEntity(123);
  ASSERT_NE(EntityHashFunction().operator()(e1), EntityHashFunction().operator()(s1));
}
TEST(CrossTypeHashTest, VariableWhile) {
  auto *e1 = new Entity(EntityType::kVariable, "123");
  auto *s1 = new StatementEntity(123);
  ASSERT_NE(EntityHashFunction().operator()(e1), EntityHashFunction().operator()(s1));
}
TEST(CrossTypeHashTest, VariablePrint) {
  auto *e1 = new Entity(EntityType::kVariable, "123");
  auto *s1 = new StatementEntity(123);
  ASSERT_NE(EntityHashFunction().operator()(e1), EntityHashFunction().operator()(s1));
}
TEST(CrossTypeHashTest, VariableRead) {
  auto *e1 = new Entity(EntityType::kVariable, "123");
  auto *s1 = new StatementEntity(123);
  ASSERT_NE(EntityHashFunction().operator()(e1), EntityHashFunction().operator()(s1));
}
TEST(CrossTypeHashTest, ConstantAssign) {
  auto *e1 = new Entity(EntityType::kConstant, "123");
  auto *s1 = new StatementEntity(123);
  ASSERT_NE(EntityHashFunction().operator()(e1), EntityHashFunction().operator()(s1));
}
TEST(CrossTypeHashTest, ConstantCall) {
  auto *e1 = new Entity(EntityType::kConstant, "123");
  auto *s1 = new StatementEntity(123);
  ASSERT_NE(EntityHashFunction().operator()(e1), EntityHashFunction().operator()(s1));
}
TEST(CrossTypeHashTest, ConstantIf) {
  auto *e1 = new Entity(EntityType::kConstant, "123");
  auto *s1 = new StatementEntity(123);
  ASSERT_NE(EntityHashFunction().operator()(e1), EntityHashFunction().operator()(s1));
}
TEST(CrossTypeHashTest, ConstantWhile) {
  auto *e1 = new Entity(EntityType::kConstant, "123");
  auto *s1 = new StatementEntity(123);
  ASSERT_NE(EntityHashFunction().operator()(e1), EntityHashFunction().operator()(s1));
}
TEST(CrossTypeHashTest, ConstantPrint) {
  auto *e1 = new Entity(EntityType::kConstant, "123");
  auto *s1 = new StatementEntity(123);
  ASSERT_NE(EntityHashFunction().operator()(e1), EntityHashFunction().operator()(s1));
}
TEST(CrossTypeHashTest, ConstantRead) {
  auto *e1 = new Entity(EntityType::kConstant, "123");
  auto *s1 = new StatementEntity(123);
  ASSERT_NE(EntityHashFunction().operator()(e1), EntityHashFunction().operator()(s1));
}
