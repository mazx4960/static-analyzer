// Copyright 2022 CS3203 Team14. All rights reserved.

#include "commons/pattern.h"
#include "gtest/gtest.h"
#include "pkb/pattern/pattern_manager.h" 
#include "pkb_test_helper.h"

using EntityStringPair = std::pair<Entity *, std::string>;
using EntityStringPairUnorderedSet = std::unordered_set<EntityStringPair, StmtExprHashFunction>; 
 
/*
* A single statement with multiple expressions
*/ 
TEST(PopulatePatternTest, SingleStatementTest) {
  auto *stmt = new AssignStmtEntity("1"); 
  auto *v = new VariableEntity("v"); 
  const auto *expr1 = "x + 1";
  const auto *expr2 = "y + 1";
  const auto *expr3 = "x + 1 + y + 1";  
  EntityStringPairUnorderedSet expected;
  auto *test_helper = new PKBTestHelper();
  std::vector<Pattern *> patterns({
      new Pattern(stmt, v, expr1),
      new Pattern(stmt, v, expr2),
      new Pattern(stmt, v, expr3), 
  });

  expected.insert(EntityStringPair(stmt, expr1));
  expected.insert(EntityStringPair(stmt, expr2));
  expected.insert(EntityStringPair(stmt, expr3));

  auto *pattern_manager = new PatternManager();
  pattern_manager->Populate(patterns);
  auto *pattern_table = pattern_manager->GetTable();
  bool compare = test_helper->set_pair_compare(pattern_table->Get(v), expected);
  ASSERT_EQ(pattern_table->GetTable().size(), 1);
  ASSERT_EQ(pattern_table->Get(v).size(), 3);   
  ASSERT_EQ(compare, true); 
}

/*
* Multiple statements with the same variable and same expression
*/ 
TEST(PopulatePatternTest, RepeatedVariableExpressionTest) {
  auto *stmt1 = new AssignStmtEntity("1");
  auto *stmt2 = new AssignStmtEntity("2");
  auto *stmt3 = new AssignStmtEntity("3");
  auto *v = new VariableEntity("v");
  const auto *expr = "x + 1";
  EntityStringPairUnorderedSet expected;
  auto *test_helper = new PKBTestHelper();
  std::vector<Pattern *> patterns({
      new Pattern(stmt1, v, expr),
      new Pattern(stmt2, v, expr),
      new Pattern(stmt3, v, expr),
  });

  expected.insert(EntityStringPair(stmt1, expr));
  expected.insert(EntityStringPair(stmt2, expr));
  expected.insert(EntityStringPair(stmt3, expr));

  auto *pattern_manager = new PatternManager();
  pattern_manager->Populate(patterns);
  auto *pattern_table = pattern_manager->GetTable();
  bool compare = test_helper->set_pair_compare(pattern_table->Get(v), expected);
  ASSERT_EQ(pattern_table->GetTable().size(), 1);
  ASSERT_EQ(pattern_table->Get(v).size(), 3);
  ASSERT_EQ(compare, true);
}

/*
* Multiple statements with the same variable and different expressions
*/ 
TEST(PopulatePatternTest, RepeatedVariableTest) {
  auto *stmt1 = new AssignStmtEntity("1");
  auto *stmt2 = new AssignStmtEntity("2");
  auto *stmt3 = new AssignStmtEntity("3");
  auto *v = new VariableEntity("v"); 
  const auto *expr1 = "x + 1";
  const auto *expr2 = "y + 2";
  const auto *expr3 = "z + 3";
  EntityStringPairUnorderedSet expected;
  auto *test_helper = new PKBTestHelper();
  std::vector<Pattern *> patterns({
      new Pattern(stmt1, v, expr1),
      new Pattern(stmt2, v, expr2),
      new Pattern(stmt3, v, expr3),
  });

  expected.insert(EntityStringPair(stmt1, expr1));
  expected.insert(EntityStringPair(stmt2, expr2));
  expected.insert(EntityStringPair(stmt3, expr3));

  auto *pattern_manager = new PatternManager();
  pattern_manager->Populate(patterns);
  auto *pattern_table = pattern_manager->GetTable();
  bool compare = test_helper->set_pair_compare(pattern_table->Get(v), expected);
  ASSERT_EQ(pattern_table->GetTable().size(), 1);
  ASSERT_EQ(pattern_table->Get(v).size(), 3);  
  ASSERT_EQ(compare, true);
} 

/*
* Multiple statements with different variables and same expression
*/ 
TEST(PopulatePatternTest, RepeatedExpressionTest) {
  auto *stmt1 = new AssignStmtEntity("1");
  auto *stmt2 = new AssignStmtEntity("2");
  auto *stmt3 = new AssignStmtEntity("3");
  auto *stmt4 = new AssignStmtEntity("4");
  auto *v1 = new VariableEntity("v1");
  auto *v2 = new VariableEntity("v2");
  auto *v3 = new VariableEntity("v3");
  auto *v4 = new VariableEntity("v4");
  const auto *expr = "x * 3"; 
  EntityStringPairUnorderedSet expected_v1;
  EntityStringPairUnorderedSet expected_v2;
  EntityStringPairUnorderedSet expected_v3;
  EntityStringPairUnorderedSet expected_v4;
  auto *test_helper = new PKBTestHelper();
  std::vector<Pattern *> patterns({
      new Pattern(stmt1, v1, expr),  
      new Pattern(stmt2, v2, expr), 
      new Pattern(stmt3, v3, expr),  
      new Pattern(stmt4, v4, expr)});

  expected_v1.insert(EntityStringPair(stmt1, expr));
  expected_v2.insert(EntityStringPair(stmt2, expr));
  expected_v3.insert(EntityStringPair(stmt3, expr));
  expected_v4.insert(EntityStringPair(stmt4, expr));

  auto *pattern_manager = new PatternManager();
  pattern_manager->Populate(patterns);
  auto *pattern_table = pattern_manager->GetTable();
  bool compare_v1 = test_helper->set_pair_compare(pattern_table->Get(v1), expected_v1);
  bool compare_v2 = test_helper->set_pair_compare(pattern_table->Get(v2), expected_v2);
  bool compare_v3 = test_helper->set_pair_compare(pattern_table->Get(v3), expected_v3);
  bool compare_v4 = test_helper->set_pair_compare(pattern_table->Get(v4), expected_v4);

  ASSERT_EQ(pattern_table->GetTable().size(), 4);
  ASSERT_EQ(pattern_table->Get(v1).size(), 1);
  ASSERT_EQ(pattern_table->Get(v2).size(), 1);
  ASSERT_EQ(pattern_table->Get(v3).size(), 1);
  ASSERT_EQ(pattern_table->Get(v4).size(), 1);
  ASSERT_EQ(compare_v1, true);
  ASSERT_EQ(compare_v2, true);
  ASSERT_EQ(compare_v3, true);
  ASSERT_EQ(compare_v4, true);
} 

/*
* Multiple statements with different variables and different expressions
*/
TEST(PopulatePatternTest, UniqueStatementTest) {
  auto *stmt1 = new AssignStmtEntity("1");
  auto *stmt2 = new AssignStmtEntity("2");
  auto *stmt3 = new AssignStmtEntity("3");
  auto *stmt4 = new AssignStmtEntity("4");
  auto *v1 = new VariableEntity("v1");
  auto *v2 = new VariableEntity("v2");
  auto *v3 = new VariableEntity("v3");
  auto *v4 = new VariableEntity("v4");
  const auto *expr1 = "x + 1";
  const auto *expr2 = "y + 2";
  const auto *expr3 = "z + 3";
  const auto *expr4 = "w + 4";
  EntityStringPairUnorderedSet expected_v1;
  EntityStringPairUnorderedSet expected_v2;
  EntityStringPairUnorderedSet expected_v3;
  EntityStringPairUnorderedSet expected_v4;
  auto *test_helper = new PKBTestHelper();
  std::vector<Pattern *> patterns({
      new Pattern(stmt1, v1, expr1), 
      new Pattern(stmt2, v2, expr2),
      new Pattern(stmt3, v3, expr3),
      new Pattern(stmt4, v4, expr4)});

  expected_v1.insert(EntityStringPair(stmt1, expr1));
  expected_v2.insert(EntityStringPair(stmt2, expr2));
  expected_v3.insert(EntityStringPair(stmt3, expr3));
  expected_v4.insert(EntityStringPair(stmt4, expr4));

  auto *pattern_manager = new PatternManager();
  pattern_manager->Populate(patterns);
  auto *pattern_table = pattern_manager->GetTable();
  bool compare_v1 = test_helper->set_pair_compare(pattern_table->Get(v1), expected_v1);
  bool compare_v2 = test_helper->set_pair_compare(pattern_table->Get(v2), expected_v2);
  bool compare_v3 = test_helper->set_pair_compare(pattern_table->Get(v3), expected_v3);
  bool compare_v4 = test_helper->set_pair_compare(pattern_table->Get(v4), expected_v4);
  ASSERT_EQ(pattern_table->GetTable().size(), 4);
  ASSERT_EQ(pattern_table->Get(v1).size(), 1);
  ASSERT_EQ(pattern_table->Get(v2).size(), 1);
  ASSERT_EQ(pattern_table->Get(v3).size(), 1);
  ASSERT_EQ(pattern_table->Get(v4).size(), 1);
  ASSERT_EQ(compare_v1, true);
  ASSERT_EQ(compare_v2, true);
  ASSERT_EQ(compare_v3, true);
  ASSERT_EQ(compare_v4, true);
}

/*
* Duplicate single statement
*/ 
TEST(PopulatePatternTest, DuplicateSingleStatementTest) {
  auto *stmt = new AssignStmtEntity("1"); 
  auto *v = new VariableEntity("v2");
  const auto *expr = "x + 1";
  EntityStringPairUnorderedSet expected;
  auto *test_helper = new PKBTestHelper();
  std::vector<Pattern *> patterns({
      new Pattern(stmt, v, expr),
      new Pattern(stmt, v, expr),
      new Pattern(stmt, v, expr),
      new Pattern(stmt, v, expr)
  });

  expected.insert(EntityStringPair(stmt, expr));

  auto *pattern_manager = new PatternManager();
  pattern_manager->Populate(patterns);
  auto *pattern_table = pattern_manager->GetTable();
  bool compare = test_helper->set_pair_compare(pattern_table->Get(v), expected); 
  ASSERT_EQ(pattern_table->GetTable().size(), 1);
  ASSERT_EQ(pattern_table->Get(v).size(), 1); 
  ASSERT_EQ(compare, true);
}

/*
* Duplicate multiple statements
*/
TEST(PopulatePatternTest, DuplicateMultipleStatementTest) {
  auto *stmt1 = new AssignStmtEntity("1");
  auto *stmt2 = new AssignStmtEntity("2"); 
  auto *v1 = new VariableEntity("v1");
  auto *v2 = new VariableEntity("v2");
  const auto *expr1 = "x + 1";
  const auto *expr2 = "y + 1";
  EntityStringPairUnorderedSet expected_v1;
  EntityStringPairUnorderedSet expected_v2; 
  auto *test_helper = new PKBTestHelper();

  std::vector<Pattern *> patterns({
      new Pattern(stmt1, v1, expr1),
      new Pattern(stmt1, v1, expr1),
      new Pattern(stmt2, v2, expr2),
      new Pattern(stmt2, v2, expr2), 
  });

  expected_v1.insert(EntityStringPair(stmt1, expr1));
  expected_v2.insert(EntityStringPair(stmt2, expr2)); 

  auto *pattern_manager = new PatternManager();
  pattern_manager->Populate(patterns);
  auto *pattern_table = pattern_manager->GetTable();
  bool compare_v1 = test_helper->set_pair_compare(pattern_table->Get(v1), expected_v1);
  bool compare_v2 = test_helper->set_pair_compare(pattern_table->Get(v2), expected_v2);  
  ASSERT_EQ(pattern_table->GetTable().size(), 2);
  ASSERT_EQ(pattern_table->Get(v1).size(), 1);
  ASSERT_EQ(pattern_table->Get(v2).size(), 1);
  ASSERT_EQ(compare_v1, true);
  ASSERT_EQ(compare_v2, true);
}

/*
* Stress test with multiple statements
*/
TEST(PopulatePatternTest, StressTest) { 
  std::vector<Pattern *> patterns;
  int length = 2000;
  int recurse = 1000;
  patterns.reserve(length);
  auto *v1 = new VariableEntity("v1");
  auto *v2 = new VariableEntity("v2");
  EntityStringPairUnorderedSet expected_v1;
  EntityStringPairUnorderedSet expected_v2;
  auto *test_helper = new PKBTestHelper();

  for (int i = 0; i < recurse; i++) { 
    auto *stmt = new AssignStmtEntity(std::to_string(i));
    std::string expr = "x + " + std::to_string(i);
    patterns.push_back(new Pattern(stmt, v1, expr)); 
    patterns.push_back(new Pattern(stmt, v2, expr));
    expected_v1.insert(EntityStringPair(stmt, expr));
    expected_v2.insert(EntityStringPair(stmt, expr)); 
  }

  auto *pattern_manager = new PatternManager();
  pattern_manager->Populate(patterns);
  auto *pattern_table = pattern_manager->GetTable();
  bool compare_v1 = test_helper->set_pair_compare(pattern_table->Get(v1), expected_v1);
  bool compare_v2 = test_helper->set_pair_compare(pattern_table->Get(v2), expected_v2);  
  ASSERT_EQ(pattern_table->GetTable().size(), 2);
  ASSERT_EQ(pattern_table->Get(v1).size(), 1000);
  ASSERT_EQ(pattern_table->Get(v2).size(), 1000);
  ASSERT_EQ(compare_v1, true);
  ASSERT_EQ(compare_v2, true);
}

TEST(PopulatePatternTest, BasicTest) {
  auto *stmt1 = new AssignStmtEntity("1");
  auto *stmt2 = new AssignStmtEntity("2");
  auto *stmt3 = new AssignStmtEntity("3");
  auto *v1 = new VariableEntity("v1");
  auto *v2 = new VariableEntity("v2");
  const auto *expr1 = "x + 1";
  const auto *expr2 = "y + 1";
  EntityStringPairUnorderedSet expected_v1;
  EntityStringPairUnorderedSet expected_v2;
  auto *test_helper = new PKBTestHelper();

  std::vector<Pattern *> patterns({
      new Pattern(stmt1, v1, expr1),
      new Pattern(stmt2, v2, expr1),
      new Pattern(stmt3, v2, expr2),
  });

  expected_v1.insert(EntityStringPair(stmt1, expr1));
  expected_v2.insert(EntityStringPair(stmt2, expr1)); 
  expected_v2.insert(EntityStringPair(stmt3, expr2)); 

  auto *pattern_manager = new PatternManager();
  pattern_manager->Populate(patterns);
  auto *pattern_table = pattern_manager->GetTable();
  ASSERT_EQ(pattern_table->GetTable().size(), 2);
  ASSERT_EQ(pattern_table->Get(v1).size(), 1);
  ASSERT_EQ(pattern_table->Get(v2).size(), 2);

  auto *stmt_actual = pattern_table->Get(v1).begin()->first;
  auto expr_actual = pattern_table->Get(v1).begin()->second;
  bool compare_v1 = test_helper->set_pair_compare(pattern_table->Get(v1), expected_v1);
  bool compare_v2 = test_helper->set_pair_compare(pattern_table->Get(v2), expected_v2);  
  ASSERT_EQ(stmt_actual, stmt1);
  ASSERT_EQ(expr_actual, expr1);
  ASSERT_EQ(compare_v1, true);
  ASSERT_EQ(compare_v2, true);
}