// Copyright 2022 CS3203 Team14. All rights reserved.

#include "gtest/gtest.h"
#include "pkb/entity/entity_table.h"
#include "pkb/pkb.h"
#include "pkb_test_helper.h"

using EntityPointerUnorderedSet = std::unordered_set<Entity *, EntityHashFunction, EntityPointerEquality>;

TEST(QueryEntityTest, VariableTest) {
  std::vector<Entity *> entities = {new VariableEntity("a"), new VariableEntity("b"), new VariableEntity("c"),
                                    new VariableEntity("d")};

  EntityPointerUnorderedSet expected;
  for (int i = 0; i < 4; i++) { expected.insert(entities[i]); }

  PKB *pkb = new PKB();
  pkb->populate(entities);
  EntityPointerUnorderedSet actual = pkb->getEntities(EntityType::kVariable);
  bool compare = PKBTestHelper::set_compare(actual, expected);
  ASSERT_EQ(actual.size(), 4);
  ASSERT_EQ(compare, true);
}

TEST(QueryEntityTest, ConstantTest) {
  std::vector<Entity *> entities = {new ConstantEntity("1"), new ConstantEntity("2"), new ConstantEntity("3"),
                                    new ConstantEntity("4")};

  EntityPointerUnorderedSet expected;
  for (int i = 0; i < 4; i++) { expected.insert(entities[i]); }

  PKB *pkb = new PKB();
  pkb->populate(entities);
  EntityPointerUnorderedSet actual = pkb->getEntities(EntityType::kConstant);
  bool compare = PKBTestHelper::set_compare(actual, expected);
  ASSERT_EQ(actual.size(), 4);
  ASSERT_EQ(compare, true);
}

TEST(QueryEntityTest, ProcedureTest) {
  std::vector<Entity *> entities = {new ProcedureEntity("One"), new ProcedureEntity("Two"),
                                    new ProcedureEntity("Three"), new ProcedureEntity("Four")};

  EntityPointerUnorderedSet expected;
  for (int i = 0; i < 4; i++) { expected.insert(entities[i]); }

  PKB *pkb = new PKB();
  pkb->populate(entities);
  EntityPointerUnorderedSet actual = pkb->getEntities(EntityType::kProcedure);
  bool compare = PKBTestHelper::set_compare(actual, expected);
  ASSERT_EQ(actual.size(), 4);
  ASSERT_EQ(compare, true);
}

TEST(QueryEntityTest, CallStatementTest) {
  std::vector<Entity *> entities = {new CallStmtEntity("1", std::string()), new CallStmtEntity("2", std::string()),
                                    new CallStmtEntity("3", std::string()), new CallStmtEntity("4", std::string())};

  EntityPointerUnorderedSet expected;
  for (int i = 0; i < 4; i++) { expected.insert(entities[i]); }

  PKB *pkb = new PKB();
  pkb->populate(entities);
  EntityPointerUnorderedSet actual = pkb->getEntities(EntityType::kCallStmt);
  bool compare = PKBTestHelper::set_compare(actual, expected);
  ASSERT_EQ(actual.size(), 4);
  ASSERT_EQ(compare, true);
}

TEST(QueryEntityTest, IfStatementTest) {
  std::vector<Entity *> entities = {new IfStmtEntity("1"), new IfStmtEntity("2"), new IfStmtEntity("3"),
                                    new IfStmtEntity("4")};

  EntityPointerUnorderedSet expected;
  for (int i = 0; i < 4; i++) { expected.insert(entities[i]); }

  PKB *pkb = new PKB();
  pkb->populate(entities);
  EntityPointerUnorderedSet actual = pkb->getEntities(EntityType::kIfStmt);
  bool compare = PKBTestHelper::set_compare(actual, expected);
  ASSERT_EQ(actual.size(), 4);
  ASSERT_EQ(compare, true);
}

TEST(QueryEntityTest, WhileStatementTest) {
  std::vector<Entity *> entities = {new WhileStmtEntity("1"), new WhileStmtEntity("2"), new WhileStmtEntity("3"),
                                    new WhileStmtEntity("4")};

  EntityPointerUnorderedSet expected;
  for (int i = 0; i < 4; i++) { expected.insert(entities[i]); }

  PKB *pkb = new PKB();
  pkb->populate(entities);
  EntityPointerUnorderedSet actual = pkb->getEntities(EntityType::kWhileStmt);
  bool compare = PKBTestHelper::set_compare(actual, expected);
  ASSERT_EQ(actual.size(), 4);
  ASSERT_EQ(compare, true);
}

TEST(QueryEntityTest, PrintStatementTest) {
  std::vector<Entity *> entities = {new PrintStmtEntity("1"), new PrintStmtEntity("2"), new PrintStmtEntity("3"),
                                    new PrintStmtEntity("4")};

  EntityPointerUnorderedSet expected;
  for (int i = 0; i < 4; i++) { expected.insert(entities[i]); }

  PKB *pkb = new PKB();
  pkb->populate(entities);
  EntityPointerUnorderedSet actual = pkb->getEntities(EntityType::kPrintStmt);
  bool compare = PKBTestHelper::set_compare(actual, expected);
  ASSERT_EQ(actual.size(), 4);
  ASSERT_EQ(compare, true);
}

TEST(QueryEntityTest, ReadStatementTest) {
  std::vector<Entity *> entities = {new ReadStmtEntity("1"), new ReadStmtEntity("2"), new ReadStmtEntity("3"),
                                    new ReadStmtEntity("4")};

  EntityPointerUnorderedSet expected;
  for (int i = 0; i < 4; i++) { expected.insert(entities[i]); }

  PKB *pkb = new PKB();
  pkb->populate(entities);
  EntityPointerUnorderedSet actual = pkb->getEntities(EntityType::kReadStmt);
  bool compare = PKBTestHelper::set_compare(actual, expected);
  ASSERT_EQ(actual.size(), 4);
  ASSERT_EQ(compare, true);
}

TEST(QueryEntityTest, AssignStatementTest) {
  std::vector<Entity *> entities = {new AssignStmtEntity("1"), new AssignStmtEntity("2"), new AssignStmtEntity("3"),
                                    new AssignStmtEntity("4")};

  EntityPointerUnorderedSet expected;
  for (int i = 0; i < 4; i++) { expected.insert(entities[i]); }

  PKB *pkb = new PKB();
  pkb->populate(entities);
  EntityPointerUnorderedSet actual = pkb->getEntities(EntityType::kAssignStmt);
  bool compare = PKBTestHelper::set_compare(actual, expected);
  ASSERT_EQ(actual.size(), 4);
  ASSERT_EQ(compare, true);
}

TEST(QueryEntityTest, SingleEntityStressTest) {
  int size = 1000;
  std::vector<Entity *> entities;
  EntityPointerUnorderedSet expected;
  for (int i = 0; i < size; i++) {
    std::string stmt = std::to_string(i);
    entities.push_back(new AssignStmtEntity(stmt));
    expected.insert(new AssignStmtEntity(stmt));
  }

  PKB *pkb = new PKB();
  pkb->populate(entities);
  EntityPointerUnorderedSet actual = pkb->getEntities(EntityType::kAssignStmt);
  bool compare = PKBTestHelper::set_compare(actual, expected);
  ASSERT_EQ(actual.size(), 1000);
  ASSERT_EQ(compare, true);
}

TEST(QueryEntityTest, MultipleEntitiesTest) {
  std::vector<Entity *> entities = {new ConstantEntity("1"),   new ConstantEntity("2"),    new ConstantEntity("3"),
                                    new VariableEntity("w"),   new VariableEntity("x"),    new VariableEntity("y"),
                                    new VariableEntity("z"),   new ProcedureEntity("One"), new ProcedureEntity("Two"),
                                    new ReadStmtEntity("1"),   new ReadStmtEntity("2"),    new ReadStmtEntity("3"),
                                    new ReadStmtEntity("4"),   new ReadStmtEntity("5"),    new AssignStmtEntity("6"),
                                    new AssignStmtEntity("7"), new AssignStmtEntity("8"),  new AssignStmtEntity("9")};

  EntityPointerUnorderedSet expected;
  for (int i = 0; i < 18; i++) { expected.insert(entities[i]); }

  PKB *pkb = new PKB();
  pkb->populate(entities);
  EntityPointerUnorderedSet actual_constant = pkb->getEntities(EntityType::kConstant);
  EntityPointerUnorderedSet actual_variable = pkb->getEntities(EntityType::kVariable);
  EntityPointerUnorderedSet actual_procedure = pkb->getEntities(EntityType::kProcedure);
  EntityPointerUnorderedSet actual_read = pkb->getEntities(EntityType::kReadStmt);
  EntityPointerUnorderedSet actual_assign = pkb->getEntities(EntityType::kAssignStmt);
  EntityPointerUnorderedSet actual;
  actual.insert(actual_constant.begin(), actual_constant.end());
  actual.insert(actual_variable.begin(), actual_variable.end());
  actual.insert(actual_procedure.begin(), actual_procedure.end());
  actual.insert(actual_read.begin(), actual_read.end());
  actual.insert(actual_assign.begin(), actual_assign.end());

  bool compare = PKBTestHelper::set_compare(actual, expected);
  ASSERT_EQ(actual.size(), 18);
  ASSERT_EQ(compare, true);
}

TEST(QueryEntityTest, MultipleEntitiesStressTest) {
  int size = 2000;
  std::vector<Entity *> entities;
  EntityPointerUnorderedSet expected;
  for (int i = 0; i < size; i = i + 2) {
    std::string one = std::to_string(i);
    std::string two = std::to_string(i + 1);
    entities.push_back(new AssignStmtEntity(one));
    entities.push_back(new PrintStmtEntity(two));
    expected.insert(new AssignStmtEntity(one));
    expected.insert(new PrintStmtEntity(two));
  }

  PKB *pkb = new PKB();
  pkb->populate(entities);
  EntityPointerUnorderedSet actual_assign = pkb->getEntities(EntityType::kAssignStmt);
  EntityPointerUnorderedSet actual_print = pkb->getEntities(EntityType::kPrintStmt);
  EntityPointerUnorderedSet actual;
  actual.insert(actual_assign.begin(), actual_assign.end());
  actual.insert(actual_print.begin(), actual_print.end());
  bool compare = PKBTestHelper::set_compare(actual, expected);
  ASSERT_EQ(actual.size(), 2000);
  ASSERT_EQ(compare, true);
}