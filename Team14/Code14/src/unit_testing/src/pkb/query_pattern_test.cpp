// Copyright 2022 CS3203 Team14. All rights reserved.

#include "commons/pattern.h"
#include "gtest/gtest.h"
#include "pkb/pattern/pattern_manager.h"

TEST(QueryPatternTest, BasicTest) {
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
  auto result = pattern_manager->Get(v2, "x");

  ASSERT_EQ(result.size(), 1);
}
