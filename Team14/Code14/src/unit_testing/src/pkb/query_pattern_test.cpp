// Copyright 2022 CS3203 Team14. All rights reserved.

#include "commons/pattern.h"
#include "gtest/gtest.h"
#include "pkb/pattern/pattern_manager.h"

TEST(QueryPatternTest, BasicTest) {
  std::vector<Pattern *> patterns({
      new Pattern(new AssignStmtEntity("1"), new VariableEntity("X"), "1"),
      new Pattern(new AssignStmtEntity("2"), new VariableEntity("X"), "2"),
      new Pattern(new AssignStmtEntity("3"), new VariableEntity("Y"), "1"),
      new Pattern(new AssignStmtEntity("4"), new VariableEntity("A"), "M"),
      new Pattern(new AssignStmtEntity("5"), new VariableEntity("A"), "M + 1"),
      new Pattern(new AssignStmtEntity("6"), new VariableEntity("B"), "N"),
      new Pattern(new AssignStmtEntity("7"), new VariableEntity("X"), "east + west"),
      new Pattern(new AssignStmtEntity("8"), new VariableEntity("Y"), "x + y + z"),
  });

  auto *pattern_manager = new PatternManager();
  pattern_manager->Populate(patterns);

  auto result_empty = pattern_manager->Get(new VariableEntity("Z"), "x + 1", true);
  auto result_expr_constant = pattern_manager->Get(new VariableEntity("Y"), "1", true);
  auto result_expr_variable = pattern_manager->Get(new VariableEntity("A"), "M", true);
  auto result_multi_stmt = pattern_manager->Get(new VariableEntity("X"), "", true);
  auto result_exact_success = pattern_manager->Get(new VariableEntity("Y"), "x + y + z", false);
  auto result_exact_failure = pattern_manager->Get(new VariableEntity("X"), "east", false);

  ASSERT_EQ(result_empty.size(), 0);
  ASSERT_EQ(result_expr_constant.size(), 1);
  ASSERT_EQ(result_expr_variable.size(), 2);
  ASSERT_EQ(result_multi_stmt.size(), 3);
  ASSERT_EQ(result_exact_success.size(), 1);
  ASSERT_EQ(result_exact_failure.size(), 0);
}
