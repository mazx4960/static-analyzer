// Copyright 2022 CS3203 Team14. All rights reserved.

#include "commons/pattern.h"
#include "gtest/gtest.h"
#include "pkb/pattern/pattern_manager.h"

TEST(PopulatePatternTest, BasicTest) {
  auto *stmt1 = new AssignStmtEntity("1");
  auto *stmt2 = new AssignStmtEntity("2");
  auto *stmt3 = new AssignStmtEntity("3");
  auto *v1 = new VariableEntity("v1");
  auto *v2 = new VariableEntity("v2");
  std::vector<Pattern *> patterns({
      new Pattern(stmt1, v1, "x + 1"),
      new Pattern(stmt2, v2, "x + 1"),
      new Pattern(stmt3, v2, "y + 1"),
  });

  auto *pattern_manager = new PatternManager();
  pattern_manager->Populate(patterns);
  auto *pattern_table_class = pattern_manager->GetTable();
  auto pattern_table = pattern_table_class->GetTable();
  ASSERT_EQ(pattern_table.size(), 2);
  ASSERT_EQ(pattern_table[v1].size(), 1);
  ASSERT_EQ(pattern_table[v2].size(), 2);

  auto *s_actual = pattern_table[v1].begin()->first;
  auto expr_actual = pattern_table[v1].begin()->second;
  ASSERT_EQ(s_actual, stmt1);
  ASSERT_EQ(expr_actual, "x + 1");
}