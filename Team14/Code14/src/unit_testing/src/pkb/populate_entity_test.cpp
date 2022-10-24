// Copyright 2022 CS3203 Team14. All rights reserved.

#include <gtest/gtest.h>

#include <vector>

#include "commons/entity.h"
#include "pkb/pkb.h"
#include "pkb_test_helper.h"

using EntityPointerUnorderedSet = std::unordered_set<Entity *, EntityHashFunction, EntityPointerEquality>;

/*
* Single entity 
*/

TEST(PopulateEntityTest, VariableTest) {
  std::vector<std::string> entity_names({"a", "b", "c", "d", "e", "f"});
  std::vector<Entity *> entities;
  EntityPointerUnorderedSet expected;
  int length = entity_names.size();
  entities.reserve(length);

  for (int i = 0; i < length; i++) {
    entities.push_back(new VariableEntity(entity_names[i]));
    expected.insert(new VariableEntity(entity_names[i]));
  }

  auto *entity_manager = new EntityManager();
  entity_manager->Populate(entities);
  auto *entity_table = entity_manager->GetTable(EntityType::kVariable);
  bool compare = PKBTestHelper::set_compare(entity_table->get(), expected);
  ASSERT_EQ(entity_table->get().size(), length);
  ASSERT_EQ(compare, true);
}

TEST(PopulateEntityTest, ConstantTest) {
  std::vector<std::string> entity_names({"1", "2", "3", "4", "5", "6"});
  std::vector<Entity *> entities;
  EntityPointerUnorderedSet expected;
  int length = entity_names.size();
  entities.reserve(length);

  for (int i = 0; i < length; i++) {
    entities.push_back(new ConstantEntity(entity_names[i]));
    expected.insert(new ConstantEntity(entity_names[i]));
  }

  auto *entity_manager = new EntityManager();
  entity_manager->Populate(entities);
  auto *entity_table = entity_manager->GetTable(EntityType::kConstant);
  bool compare = PKBTestHelper::set_compare(entity_table->get(), expected);
  ASSERT_EQ(entity_table->get().size(), length);
  ASSERT_EQ(compare, true);
}

TEST(PopulateEntityTest, ProcedureTest) {
  std::vector<std::string> procedure_names({"a", "b", "c", "d", "e", "f"});
  std::vector<Entity *> entities;
  EntityPointerUnorderedSet expected;
  int length = procedure_names.size();
  entities.reserve(length);

  for (int i = 0; i < length; i++) {
    entities.push_back(new ProcedureEntity(procedure_names[i]));
    expected.insert(new ProcedureEntity(procedure_names[i]));
  }

  auto *entity_manager = new EntityManager();
  entity_manager->Populate(entities);
  auto *entity_table = entity_manager->GetTable(EntityType::kProcedure);
  bool compare = PKBTestHelper::set_compare(entity_table->get(), expected);
  ASSERT_EQ(entity_table->get().size(), length);
  ASSERT_EQ(compare, true);
}

TEST(PopulateEntityTest, CallStatementTest) {
  std::vector<std::string> stmt_no({"1", "2", "3", "4", "5", "6"});
  std::vector<Entity *> entities;
  int length = stmt_no.size();
  entities.reserve(length);
  EntityPointerUnorderedSet expected;

  for (int i = 0; i < length; i++) {
    entities.push_back(new CallStmtEntity(stmt_no[i], std::string()));
    expected.insert(new CallStmtEntity(stmt_no[i], std::string()));
  }

  auto *entity_manager = new EntityManager();
  entity_manager->Populate(entities);
  auto *entity_table = entity_manager->GetTable(EntityType::kCallStmt);
  bool compare = PKBTestHelper::set_compare(entity_table->get(), expected);
  ASSERT_EQ(entity_table->get().size(), length);
  ASSERT_EQ(compare, true);
}

TEST(PopulateEntityTest, IfStatementTest) {
  std::vector<std::string> stmt_no({"1", "2", "3", "4", "5", "6"});
  std::vector<Entity *> entities;
  int length = stmt_no.size();
  entities.reserve(length);
  EntityPointerUnorderedSet expected;

  for (int i = 0; i < length; i++) {
    entities.push_back(new IfStmtEntity(stmt_no[i]));
    expected.insert(new IfStmtEntity(stmt_no[i]));
  }

  auto *entity_manager = new EntityManager();
  entity_manager->Populate(entities);
  auto *entity_table = entity_manager->GetTable(EntityType::kIfStmt);
  bool compare = PKBTestHelper::set_compare(entity_table->get(), expected);
  ASSERT_EQ(entity_table->get().size(), length);
  ASSERT_EQ(compare, true);
}

TEST(PopulateEntityTest, WhileStatementTest) {
  std::vector<std::string> stmt_no({"1", "2", "3", "4", "5", "6"});
  std::vector<Entity *> entities;
  int length = stmt_no.size();
  entities.reserve(length);
  EntityPointerUnorderedSet expected;

  for (int i = 0; i < length; i++) {
    entities.push_back(new WhileStmtEntity(stmt_no[i]));
    expected.insert(new WhileStmtEntity(stmt_no[i]));
  }

  auto *entity_manager = new EntityManager();
  entity_manager->Populate(entities);
  auto *entity_table = entity_manager->GetTable(EntityType::kWhileStmt);
  bool compare = PKBTestHelper::set_compare(entity_table->get(), expected);
  ASSERT_EQ(entity_table->get().size(), length);
  ASSERT_EQ(compare, true);
}

TEST(PopulateEntityTest, PrintStatementTest) {
  std::vector<std::string> stmt_no({"1", "2", "3", "4", "5", "6"});
  std::vector<Entity *> entities;
  int length = stmt_no.size();
  entities.reserve(length);
  EntityPointerUnorderedSet expected;

  for (int i = 0; i < length; i++) {
    entities.push_back(new PrintStmtEntity(stmt_no[i], std::string()));
    expected.insert(new PrintStmtEntity(stmt_no[i], std::string()));
  }

  auto *entity_manager = new EntityManager();
  entity_manager->Populate(entities);
  auto *entity_table = entity_manager->GetTable(EntityType::kPrintStmt);
  bool compare = PKBTestHelper::set_compare(entity_table->get(), expected);
  ASSERT_EQ(entity_table->get().size(), length);
  ASSERT_EQ(compare, true);
}

TEST(PopulateEntityTest, ReadStatementTest) {
  std::vector<std::string> stmt_no({"1", "2", "3", "4", "5", "6"});
  std::vector<Entity *> entities;
  int length = stmt_no.size();
  entities.reserve(length);
  EntityPointerUnorderedSet expected;

  for (int i = 0; i < length; i++) {
    entities.push_back(new ReadStmtEntity(stmt_no[i], std::string()));
    expected.insert(new ReadStmtEntity(stmt_no[i], std::string()));
  }

  auto *entity_manager = new EntityManager();
  entity_manager->Populate(entities);
  auto *entity_table = entity_manager->GetTable(EntityType::kReadStmt);
  bool compare = PKBTestHelper::set_compare(entity_table->get(), expected);
  ASSERT_EQ(entity_table->get().size(), length);
  ASSERT_EQ(compare, true);
}

TEST(PopulateEntityTest, AssignStatementTest) {
  std::vector<std::string> stmt_no({"1", "2", "3", "4", "5", "6"});
  std::vector<Entity *> entities;
  int length = stmt_no.size();
  entities.reserve(length);
  EntityPointerUnorderedSet expected;

  for (int i = 0; i < length; i++) {
    entities.push_back(new AssignStmtEntity(stmt_no[i]));
    expected.insert(new AssignStmtEntity(stmt_no[i]));

  }

  auto *entity_manager = new EntityManager();
  entity_manager->Populate(entities);
  auto *entity_table = entity_manager->GetTable(EntityType::kAssignStmt);
  bool compare = PKBTestHelper::set_compare(entity_table->get(), expected);
  ASSERT_EQ(entity_table->get().size(), length);
  ASSERT_EQ(compare, true);
}

TEST(PopulateEntityTest, SingleEntityStressTest) {
  std::vector<Entity *> entities;
  int length = 1000;
  entities.reserve(length);
  EntityPointerUnorderedSet expected;
  for (int i = 0; i < length; i++) {
    std::string stmt_no = std::to_string(i);
    entities.push_back(new AssignStmtEntity(stmt_no));
    expected.insert(new AssignStmtEntity(stmt_no));

  }
  auto *entity_manager = new EntityManager();
  entity_manager->Populate(entities);
  auto *entity_table = entity_manager->GetTable(EntityType::kAssignStmt);
  bool compare = PKBTestHelper::set_compare(entity_table->get(), expected);
  ASSERT_EQ(entity_table->get().size(), 1000);
  ASSERT_EQ(compare, true);
}

TEST(PopulateEntityTest, DuplicateVariableEntityTest) {
  std::vector<std::string> entity_names({"a", "a", "a", "a", "a", "a"});
  std::vector<Entity *> entities;
  int length = entity_names.size();
  entities.reserve(length);
  EntityPointerUnorderedSet expected;

  for (int i = 0; i < length; i++) {
    entities.push_back(new VariableEntity(entity_names[i]));
    expected.insert(new VariableEntity(entity_names[i]));
  }

  auto *entity_manager = new EntityManager();
  entity_manager->Populate(entities);
  auto *entity_table = entity_manager->GetTable(EntityType::kVariable);
  bool compare = PKBTestHelper::set_compare(entity_table->get(), expected);
  ASSERT_EQ(entity_table->get().size(), 1);
  ASSERT_EQ(compare, true);
}

TEST(PopulateEntityTest, DuplicateStatementEntityTest) {
  std::vector<std::string> stmt_no({"1", "1", "1", "1", "1", "1"});
  std::vector<Entity *> entities;
  int length = stmt_no.size();
  entities.reserve(length);
  EntityPointerUnorderedSet expected;

  for (int i = 0; i < length; i++) {
    entities.push_back(new AssignStmtEntity(stmt_no[i]));
    expected.insert(new AssignStmtEntity(stmt_no[i]));
  }

  auto *entity_manager = new EntityManager();
  entity_manager->Populate(entities);
  auto *entity_table = entity_manager->GetTable(EntityType::kAssignStmt);
  bool compare = PKBTestHelper::set_compare(entity_table->get(), expected);
  ASSERT_EQ(entity_table->get().size(), 1);
  ASSERT_EQ(compare, true);
}

/*
* Mixed entities 
*/

TEST(PopulateEntityTest, MultipleEntitiesTest) {
  std::vector<Entity *> entities;
  std::vector<std::string> variable_entity_names({"a", "b", "c", "d"});
  std::vector<std::string> constant_entity_names({"30", "31", "32", "33", "34"});
  std::vector<std::string> procedure_entity_names({"one", "two", "three"});
  std::vector<std::string> assign_stmt_no({"1", "2", "3", "4", "5"});
  std::vector<std::string> read_stmt_no({"7", "8", "9", "10"});
  std::vector<std::string> if_stmt_no({"11", "12", "13"});
  std::vector<std::string> while_stmt_no({"14"});
  EntityPointerUnorderedSet expected_variable;
  EntityPointerUnorderedSet expected_constant;
  EntityPointerUnorderedSet expected_procedure;
  EntityPointerUnorderedSet expected_assign;
  EntityPointerUnorderedSet expected_read;
  EntityPointerUnorderedSet expected_if;
  EntityPointerUnorderedSet expected_while;

  int length = variable_entity_names.size() +
      constant_entity_names.size() +
      procedure_entity_names.size() +
      assign_stmt_no.size() +
      read_stmt_no.size() +
      if_stmt_no.size() +
      while_stmt_no.size();
  entities.reserve(length);
  for (const auto &s : variable_entity_names) {
    entities.push_back(new VariableEntity(s));
    expected_variable.insert(new VariableEntity(s));
  }
  for (const auto &s : constant_entity_names) {
    entities.push_back(new ConstantEntity(s));
    expected_constant.insert(new ConstantEntity(s));
  }
  for (const auto &s : procedure_entity_names) {
    entities.push_back(new ProcedureEntity(s));
    expected_procedure.insert(new ProcedureEntity(s));
  }
  for (const auto &s : assign_stmt_no) {
    entities.push_back(new AssignStmtEntity(s));
    expected_assign.insert(new AssignStmtEntity(s));
  }
  for (const auto &s : read_stmt_no) {
    entities.push_back(new ReadStmtEntity(s, std::string()));
    expected_read.insert(new ReadStmtEntity(s, std::string()));
  }
  for (const auto &s : if_stmt_no) {
    entities.push_back(new IfStmtEntity(s));
    expected_if.insert(new IfStmtEntity(s));
  }

  for (const auto &s : while_stmt_no) {
    entities.push_back(new WhileStmtEntity(s));
    expected_while.insert(new WhileStmtEntity(s));
  }

  auto *entity_manager = new EntityManager();
  entity_manager->Populate(entities);
  auto *variable_table = entity_manager->GetTable(EntityType::kVariable);
  auto *constant_table = entity_manager->GetTable(EntityType::kConstant);
  auto *procedure_table = entity_manager->GetTable(EntityType::kProcedure);
  auto *assign_table = entity_manager->GetTable(EntityType::kAssignStmt);
  auto *read_table = entity_manager->GetTable(EntityType::kReadStmt);
  auto *if_table = entity_manager->GetTable(EntityType::kIfStmt);
  auto *while_table = entity_manager->GetTable(EntityType::kWhileStmt);

  ASSERT_TRUE(PKBTestHelper::set_compare(variable_table->get(), expected_variable));
  ASSERT_TRUE(PKBTestHelper::set_compare(constant_table->get(), expected_constant));
  ASSERT_TRUE(PKBTestHelper::set_compare(procedure_table->get(), expected_procedure));
  ASSERT_TRUE(PKBTestHelper::set_compare(assign_table->get(), expected_assign));
  ASSERT_TRUE(PKBTestHelper::set_compare(read_table->get(), expected_read));
  ASSERT_TRUE(PKBTestHelper::set_compare(if_table->get(), expected_if));
  ASSERT_TRUE(PKBTestHelper::set_compare(while_table->get(), expected_while));

  ASSERT_EQ(variable_table->get().size(), 4);
  ASSERT_EQ(constant_table->get().size(), 5);
  ASSERT_EQ(procedure_table->get().size(), 3);
  ASSERT_EQ(assign_table->get().size(), 5);
  ASSERT_EQ(read_table->get().size(), 4);
  ASSERT_EQ(if_table->get().size(), 3);
  ASSERT_EQ(while_table->get().size(), 1);
}

TEST(PopulateEntityTest, MixedEntityStressTest) {
  std::vector<Entity *> entities;
  int length = 2000;
  entities.reserve(length);
  EntityPointerUnorderedSet expected_assign;
  EntityPointerUnorderedSet expected_read;

  for (int i = 0; i < length; i = i + 2) {
    std::string assign_stmt_no = std::to_string(i);
    std::string read_stmt_no = std::to_string(i + 1);
    entities.push_back(new AssignStmtEntity(assign_stmt_no));
    entities.push_back(new ReadStmtEntity(read_stmt_no, std::string()));
    expected_assign.insert(new AssignStmtEntity(assign_stmt_no));
    expected_read.insert(new ReadStmtEntity(read_stmt_no, std::string()));
  }
  auto *entity_manager = new EntityManager();
  entity_manager->Populate(entities);
  auto *assign_stmt_table = entity_manager->GetTable(EntityType::kAssignStmt);
  auto *read_stmt_table = entity_manager->GetTable(EntityType::kReadStmt);
  bool compare_assign = PKBTestHelper::set_compare(assign_stmt_table->get(), expected_assign);
  bool compare_read = PKBTestHelper::set_compare(read_stmt_table->get(), expected_read);

  ASSERT_EQ(assign_stmt_table->get().size(), 1000);
  ASSERT_EQ(read_stmt_table->get().size(), 1000);
  ASSERT_EQ(compare_assign, true);
  ASSERT_EQ(compare_read, true);
}

TEST(PopulateEntityTest, MultipleDuplicateEntitiesTest) {
  std::vector<Entity *> entities;
  std::vector<std::string> variable_entity_names({"a", "b", "c", "d"});
  std::vector<std::string> constant_entity_names({"30", "31", "32", "33", "34"});
  std::vector<std::string> procedure_entity_names({"one", "two", "three"});
  std::vector<std::string> assign_stmt_no({"1", "2", "3", "4", "5"});
  std::vector<std::string> read_stmt_no({"7", "8", "9", "10"});
  std::vector<std::string> if_stmt_no({"11", "12", "13"});
  std::vector<std::string> while_stmt_no({"14"});
  EntityPointerUnorderedSet expected_variable;
  EntityPointerUnorderedSet expected_constant;
  EntityPointerUnorderedSet expected_procedure;
  EntityPointerUnorderedSet expected_assign;
  EntityPointerUnorderedSet expected_read;
  EntityPointerUnorderedSet expected_if;
  EntityPointerUnorderedSet expected_while;

  for (const auto &s : variable_entity_names) {
    entities.push_back(new VariableEntity(s));
    entities.push_back(new VariableEntity(s));
    entities.push_back(new VariableEntity(s));
    expected_variable.insert(new VariableEntity(s));
  }
  for (const auto &s : constant_entity_names) {
    entities.push_back(new ConstantEntity(s));
    entities.push_back(new ConstantEntity(s));
    entities.push_back(new ConstantEntity(s));
    expected_constant.insert(new ConstantEntity(s));
  }
  for (const auto &s : procedure_entity_names) {
    entities.push_back(new ProcedureEntity(s));
    entities.push_back(new ProcedureEntity(s));
    expected_procedure.insert(new ProcedureEntity(s));
  }
  for (const auto &s : assign_stmt_no) {
    entities.push_back(new AssignStmtEntity(s));
    entities.push_back(new AssignStmtEntity(s));
    entities.push_back(new AssignStmtEntity(s));
    expected_assign.insert(new AssignStmtEntity(s));
  }
  for (const auto &s : read_stmt_no) {
    entities.push_back(new ReadStmtEntity(s, std::string()));
    entities.push_back(new ReadStmtEntity(s, std::string()));
    entities.push_back(new ReadStmtEntity(s, std::string()));
    expected_read.insert(new ReadStmtEntity(s, std::string()));
  }
  for (const auto &s : if_stmt_no) {
    entities.push_back(new IfStmtEntity(s));
    entities.push_back(new IfStmtEntity(s));
    entities.push_back(new IfStmtEntity(s));
    entities.push_back(new IfStmtEntity(s));
    expected_if.insert(new IfStmtEntity(s));
  }
  for (const auto &s : while_stmt_no) {
    entities.push_back(new WhileStmtEntity(s));
    entities.push_back(new WhileStmtEntity(s));
    expected_while.insert(new WhileStmtEntity(s));
  }

  auto *entity_manager = new EntityManager();
  entity_manager->Populate(entities);
  auto *variable_table = entity_manager->GetTable(EntityType::kVariable);
  auto *constant_table = entity_manager->GetTable(EntityType::kConstant);
  auto *procedure_table = entity_manager->GetTable(EntityType::kProcedure);
  auto *assign_table = entity_manager->GetTable(EntityType::kAssignStmt);
  auto *read_table = entity_manager->GetTable(EntityType::kReadStmt);
  auto *if_table = entity_manager->GetTable(EntityType::kIfStmt);
  auto *while_table = entity_manager->GetTable(EntityType::kWhileStmt);

  ASSERT_TRUE(PKBTestHelper::set_compare(variable_table->get(), expected_variable));
  ASSERT_TRUE(PKBTestHelper::set_compare(constant_table->get(), expected_constant));
  ASSERT_TRUE(PKBTestHelper::set_compare(procedure_table->get(), expected_procedure));
  ASSERT_TRUE(PKBTestHelper::set_compare(assign_table->get(), expected_assign));
  ASSERT_TRUE(PKBTestHelper::set_compare(read_table->get(), expected_read));
  ASSERT_TRUE(PKBTestHelper::set_compare(if_table->get(), expected_if));
  ASSERT_TRUE(PKBTestHelper::set_compare(while_table->get(), expected_while));

  ASSERT_EQ(variable_table->get().size(), 4);
  ASSERT_EQ(constant_table->get().size(), 5);
  ASSERT_EQ(procedure_table->get().size(), 3);
  ASSERT_EQ(assign_table->get().size(), 5);
  ASSERT_EQ(read_table->get().size(), 4);
  ASSERT_EQ(if_table->get().size(), 3);
  ASSERT_EQ(while_table->get().size(), 1);
}