// Copyright 2022 CS3203 Team14. All rights reserved.

#include "commons/pattern.h"
#include "gtest/gtest.h"
#include "pkb/pattern/pattern_manager.h" 

// A single statement with multiple expressions
TEST(PopulatePatternTest, SingleStatementTest) {
  auto *stmt = new AssignStmtEntity("1"); 
  auto *v = new VariableEntity("v"); 
  auto expr1 = "x + 1";
  auto expr2 = "y + 1";
  auto expr3 = "x + 1 + y + 1";  
  std::vector<Pattern *> patterns({
      new Pattern(stmt, v, expr1),
      new Pattern(stmt, v, expr2),
      new Pattern(stmt, v, expr3),
  });

  auto *pattern_manager = new PatternManager();
  pattern_manager->Populate(patterns);
  auto *pattern_table = pattern_manager->GetTable();
  ASSERT_EQ(pattern_table->GetTable().size(), 1);
  ASSERT_EQ(pattern_table->Get(v).size(), 3);   
}

// Multiple statements with the same variable and expression
TEST(PopulatePatternTest, RepeatedExpressionsTest) {
  auto *stmt1 = new AssignStmtEntity("1");
  auto *stmt2 = new AssignStmtEntity("2");
  auto *stmt3 = new AssignStmtEntity("3");
  auto *v = new VariableEntity("v");
  auto expr = "x + 1";  
  std::vector<Pattern *> patterns({
      new Pattern(stmt1, v, expr),
      new Pattern(stmt2, v, expr),
      new Pattern(stmt3, v, expr),
  });

  auto *pattern_manager = new PatternManager();
  pattern_manager->Populate(patterns);
  auto *pattern_table = pattern_manager->GetTable();
  ASSERT_EQ(pattern_table->GetTable().size(), 1);
  ASSERT_EQ(pattern_table->Get(v).size(), 3);
}

// Multiple statements with the same variable and different expressions
TEST(PopulatePatternTest, SingleVariableTest) {
  auto *stmt1 = new AssignStmtEntity("1");
  auto *stmt2 = new AssignStmtEntity("2");
  auto *stmt3 = new AssignStmtEntity("3");
  auto *v = new VariableEntity("v"); 
  auto expr1 = "x + 1";
  auto expr2 = "y + 2";
  auto expr3 = "z + 3"; 
  std::vector<Pattern *> patterns({
      new Pattern(stmt1, v, expr1),
      new Pattern(stmt2, v, expr2),
      new Pattern(stmt3, v, expr3),
  });

  auto *pattern_manager = new PatternManager();
  pattern_manager->Populate(patterns);
  auto *pattern_table = pattern_manager->GetTable();
  ASSERT_EQ(pattern_table->GetTable().size(), 1);
  ASSERT_EQ(pattern_table->Get(v).size(), 3);  
}

// Multiple statements with different variables
TEST(PopulatePatternTest, MultipleVariablesTest) {
  auto *stmt1 = new AssignStmtEntity("1");
  auto *stmt2 = new AssignStmtEntity("2");
  auto *stmt3 = new AssignStmtEntity("3");
  auto *stmt4 = new AssignStmtEntity("4");
  auto *v1 = new VariableEntity("v1");
  auto *v2 = new VariableEntity("v2");
  auto expr1 = "x + 1";
  auto expr2 = "y + 2";
  auto expr3 = "z + 3";
  auto expr4 = "w + 4";
  std::vector<Pattern *> patterns({
      new Pattern(stmt1, v1, expr1),
      new Pattern(stmt2, v1, expr2), 
      new Pattern(stmt3, v2, expr3),
      new Pattern(stmt4, v2, expr4)
  });

  auto *pattern_manager = new PatternManager();
  pattern_manager->Populate(patterns);
  auto *pattern_table = pattern_manager->GetTable();
  ASSERT_EQ(pattern_table->GetTable().size(), 2);
  ASSERT_EQ(pattern_table->Get(v1).size(), 2);
  ASSERT_EQ(pattern_table->Get(v2).size(), 2); 
} 

// Duplicate patterns should not be added
TEST(PopulatePatternTest, DuplicateTest) {
  auto *stmt1 = new AssignStmtEntity("1");
  auto *stmt2 = new AssignStmtEntity("2"); 
  auto *v1 = new VariableEntity("v1");
  auto *v2 = new VariableEntity("v2");
  auto expr1 = "x + 1";
  auto expr2 = "y + 1";
  std::vector<Pattern *> patterns({
      new Pattern(stmt1, v1, expr1),
      new Pattern(stmt1, v1, expr1),
      new Pattern(stmt2, v2, expr2),
      new Pattern(stmt2, v2, expr2)
  });

  auto *pattern_manager = new PatternManager();
  pattern_manager->Populate(patterns);
  auto *pattern_table = pattern_manager->GetTable();
  ASSERT_EQ(pattern_table->GetTable().size(), 2);
  ASSERT_EQ(pattern_table->Get(v1).size(), 1);
  ASSERT_EQ(pattern_table->Get(v2).size(), 1); 
}

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
  auto *pattern_table = pattern_manager->GetTable();
  ASSERT_EQ(pattern_table->GetTable().size(), 2);
  ASSERT_EQ(pattern_table->Get(v1).size(), 1);
  ASSERT_EQ(pattern_table->Get(v2).size(), 2);

  auto *stmt_actual = pattern_table->Get(v1).begin()->first;
  auto expr_actual = pattern_table->Get(v1).begin()->second;
  ASSERT_EQ(stmt_actual, stmt1);
  ASSERT_EQ(expr_actual, "x + 1");
}