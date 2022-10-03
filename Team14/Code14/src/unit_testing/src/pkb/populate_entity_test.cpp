// Copyright 2022 CS3203 Team14. All rights reserved.

#include <gtest/gtest.h>

#include <vector>

#include "commons/entity.h"
#include "pkb/pkb.h"

/*
* Single entity 
*/

TEST(PopulateEntityTest, VariableTest) {
  std::vector<std::string> entity_names({"a", "b", "c", "d", "e", "f"});
  std::vector<Entity *> entities;
  int length = entity_names.size();
  entities.reserve(length);

  for (int i = 0; i < length; i++) { 
	entities.push_back(new VariableEntity(entity_names[i])); 
  }

  auto *entity_manager = new EntityManager();
  entity_manager->Populate(entities);
  auto *entity_table = entity_manager->GetTable(EntityType::kVariable);
  ASSERT_EQ(entity_table->get().size(), length);
}

TEST(PopulateEntityTest, ConstantTest) {
  std::vector<std::string> entity_names({"a", "b", "c", "d", "e", "f"});
  std::vector<Entity *> entities;
  int length = entity_names.size();
  entities.reserve(length);

  for (int i = 0; i < length; i++) { 
	entities.push_back(new ConstantEntity(entity_names[i])); 
  }

  auto *entity_manager = new EntityManager();
  entity_manager->Populate(entities);
  auto *entity_table = entity_manager->GetTable(EntityType::kConstant);
  ASSERT_EQ(entity_table->get().size(), length);
}

TEST(PopulateEntityTest, ProcedureTest) {
  std::vector<std::string> procedure_names({"a", "b", "c", "d", "e", "f"});
  std::vector<Entity *> entities;
  int length = procedure_names.size();
  entities.reserve(length);

  for (int i = 0; i < length; i++) { 
	entities.push_back(new ProcedureEntity(procedure_names[i])); 
  }

  auto *entity_manager = new EntityManager();
  entity_manager->Populate(entities);
  auto *entity_table = entity_manager->GetTable(EntityType::kProcedure);
  ASSERT_EQ(entity_table->get().size(), length);
}

TEST(PopulateEntityTest, CallStatementTest) {
  std::vector<std::string> stmt_no({"1", "2", "3", "4", "5", "6"});
  std::vector<Entity *> entities;
  int length = stmt_no.size();
  entities.reserve(length);

  for (int i = 0; i < length; i++) { 
	entities.push_back(new CallStmtEntity(stmt_no[i])); 
  }

  auto *entity_manager = new EntityManager();
  entity_manager->Populate(entities);
  auto *entity_table = entity_manager->GetTable(EntityType::kCallStmt);
  ASSERT_EQ(entity_table->get().size(), length);
}

TEST(PopulateEntityTest, IfStatementTest) {
  std::vector<std::string> stmt_no({"1", "2", "3", "4", "5", "6"});
  std::vector<Entity *> entities;
  int length = stmt_no.size();
  entities.reserve(length);

  for (int i = 0; i < length; i++) {
	entities.push_back(new IfStmtEntity(stmt_no[i]));
  }

  auto *entity_manager = new EntityManager();
  entity_manager->Populate(entities);
  auto *entity_table = entity_manager->GetTable(EntityType::kIfStmt);
  ASSERT_EQ(entity_table->get().size(), length);
}

TEST(PopulateEntityTest, WhileStatementTest) {
  std::vector<std::string> stmt_no({"1", "2", "3", "4", "5", "6"});
  std::vector<Entity *> entities;
  int length = stmt_no.size();
  entities.reserve(length);

  for (int i = 0; i < length; i++) {
	entities.push_back(new WhileStmtEntity(stmt_no[i]));
  }

  auto *entity_manager = new EntityManager();
  entity_manager->Populate(entities);
  auto *entity_table = entity_manager->GetTable(EntityType::kWhileStmt);
  ASSERT_EQ(entity_table->get().size(), length);
}

TEST(PopulateEntityTest, PrintStatementTest) {
  std::vector<std::string> stmt_no({"1", "2", "3", "4", "5", "6"});
  std::vector<Entity *> entities;
  int length = stmt_no.size();
  entities.reserve(length);

  for (int i = 0; i < length; i++) { 
	entities.push_back(new PrintStmtEntity(stmt_no[i]));
  }

  auto *entity_manager = new EntityManager();
  entity_manager->Populate(entities);
  auto *entity_table = entity_manager->GetTable(EntityType::kPrintStmt);
  ASSERT_EQ(entity_table->get().size(), length);
}

TEST(PopulateEntityTest, ReadStatementTest) {
  std::vector<std::string> stmt_no({"1", "2", "3", "4", "5", "6"});
  std::vector<Entity *> entities;
  int length = stmt_no.size();
  entities.reserve(length);

  for (int i = 0; i < length; i++) { 
	entities.push_back(new ReadStmtEntity(stmt_no[i]));
  }

  auto *entity_manager = new EntityManager();
  entity_manager->Populate(entities);
  auto *entity_table = entity_manager->GetTable(EntityType::kReadStmt);
  ASSERT_EQ(entity_table->get().size(), length); 
}

TEST(PopulateEntityTest, AssignStatementTest) {
  std::vector<std::string> stmt_no({"1", "2", "3", "4", "5", "6"});
  std::vector<Entity *> entities;
  int length = stmt_no.size();
  entities.reserve(length);

  for (int i = 0; i < length; i++) { 
	entities.push_back(new AssignStmtEntity(stmt_no[i])); 
  }

  auto *entity_manager = new EntityManager();
  entity_manager->Populate(entities);
  auto *entity_table = entity_manager->GetTable(EntityType::kAssignStmt);
  ASSERT_EQ(entity_table->get().size(), length);
}

TEST(PopulateEntityTest, SingleEntityStressTest) {
  std::vector<Entity *> entities;
  int length = 1000;
  entities.reserve(length);
  for (int i = 0; i < length; i++) {
    std::string stmt_no = std::to_string(i);
    entities.push_back(new AssignStmtEntity(stmt_no));
  }
  auto *entity_manager = new EntityManager();
  entity_manager->Populate(entities);
  auto *entity_table = entity_manager->GetTable(EntityType::kAssignStmt);
  ASSERT_EQ(entity_table->get().size(), 1000);
}

TEST(PopulateEntityTest, DuplicateVariableEntityTest) {
  std::vector<std::string> entity_names({"a", "a", "a", "a", "a", "a"});
  std::vector<Entity *> entities;
  int length = entity_names.size();
  entities.reserve(length);

  for (int i = 0; i < length; i++) {
	entities.push_back(new VariableEntity(entity_names[i]));
  }

  auto *entity_manager = new EntityManager();
  entity_manager->Populate(entities);
  auto *entity_table = entity_manager->GetTable(EntityType::kVariable);
  ASSERT_EQ(entity_table->get().size(), 1);
}

TEST(PopulateEntityTest, DuplicateStatementEntityTest) {
  std::vector<std::string> stmt_no({"1", "1", "1", "1", "1", "1"});
  std::vector<Entity *> entities;
  int length = stmt_no.size();
  entities.reserve(length);

  for (int i = 0; i < length; i++) { 
	entities.push_back(new AssignStmtEntity(stmt_no[i])); 
  }

  auto *entity_manager = new EntityManager();
  entity_manager->Populate(entities);
  auto *entity_table = entity_manager->GetTable(EntityType::kAssignStmt);
  ASSERT_EQ(entity_table->get().size(), 1);
}

/*
* Mixed entities 
*/

TEST(PopulateEntityTest, MultipleEntitiesTest) {
  std::vector<Entity *> entities;  
  std::vector<std::string> variable_entity_names({"a", "b", "c", "d"});
  std::vector<std::string> constant_entity_names({"v", "w", "x", "y", "z"});
  std::vector<std::string> procedure_entity_names({"one", "two", "three"});
  std::vector<std::string> assign_stmt_no({"1", "2", "3", "4", "5"});
  std::vector<std::string> read_stmt_no({"7", "8", "9", "10"});
  std::vector<std::string> if_stmt_no({"11", "12", "13"}); 
  std::vector<std::string> while_stmt_no({"14"}); 
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
  }
  for (const auto &s : constant_entity_names) { 
	entities.push_back(new ConstantEntity(s));
  }
  for (const auto &s : procedure_entity_names) { 
	entities.push_back(new ProcedureEntity(s)); 
  } 
  for (const auto &s : assign_stmt_no) { 
	entities.push_back(new AssignStmtEntity(s)); 
  }
  for (const auto &s : read_stmt_no) { 
	entities.push_back(new ReadStmtEntity(s)); 
  }
  for (const auto &s : if_stmt_no) { 
	entities.push_back(new IfStmtEntity(s)); 
  }
  for (const auto &s : while_stmt_no) { 
	entities.push_back(new WhileStmtEntity(s)); 
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
  for (int i = 0; i < length; i = i + 2) {
    std::string assign_stmt_no = std::to_string(i);
    std::string read_stmt_no = std::to_string(i + 1);
    entities.push_back(new AssignStmtEntity(assign_stmt_no));
    entities.push_back(new ReadStmtEntity(read_stmt_no));
  }
  auto *entity_manager = new EntityManager();
  entity_manager->Populate(entities);
  auto *assign_stmt_table = entity_manager->GetTable(EntityType::kAssignStmt);
  auto *read_stmt_table = entity_manager->GetTable(EntityType::kReadStmt);
  ASSERT_EQ(assign_stmt_table->get().size(), 1000);
  ASSERT_EQ(read_stmt_table->get().size(), 1000);
} 

TEST(PopulateEntityTest, MultipleDuplicateEntitiesTest) {
  std::vector<Entity *> entities;  
  std::vector<std::string> variable_entity_names({"a", "b", "c", "d"});
  std::vector<std::string> constant_entity_names({"v", "w", "x", "y", "z"});
  std::vector<std::string> procedure_entity_names({"one", "two", "three"});
  std::vector<std::string> assign_stmt_no({"1", "2", "3", "4", "5"});
  std::vector<std::string> read_stmt_no({"7", "8", "9", "10"});
  std::vector<std::string> if_stmt_no({"11", "12", "13"}); 
  std::vector<std::string> while_stmt_no({"14"}); 
  for (const auto &s : variable_entity_names) { 
	entities.push_back(new VariableEntity(s)); 
	entities.push_back(new VariableEntity(s)); 
	entities.push_back(new VariableEntity(s)); 
  }
  for (const auto &s : constant_entity_names) { 
	entities.push_back(new ConstantEntity(s));
    entities.push_back(new ConstantEntity(s));
    entities.push_back(new ConstantEntity(s));
  }
  for (const auto &s : procedure_entity_names) { 
	entities.push_back(new ProcedureEntity(s));
    entities.push_back(new ProcedureEntity(s));
  } 
  for (const auto &s : assign_stmt_no) { 
	entities.push_back(new AssignStmtEntity(s));
    entities.push_back(new AssignStmtEntity(s));
    entities.push_back(new AssignStmtEntity(s));
  }
  for (const auto &s : read_stmt_no) { 
	entities.push_back(new ReadStmtEntity(s));
    entities.push_back(new ReadStmtEntity(s)); 
	entities.push_back(new ReadStmtEntity(s));   
  }
  for (const auto &s : if_stmt_no) { 
	entities.push_back(new IfStmtEntity(s)); 
	entities.push_back(new IfStmtEntity(s));
    entities.push_back(new IfStmtEntity(s));
    entities.push_back(new IfStmtEntity(s)); 
  }
  for (const auto &s : while_stmt_no) { 
	entities.push_back(new WhileStmtEntity(s));
    entities.push_back(new WhileStmtEntity(s)); 
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
  ASSERT_EQ(variable_table->get().size(), 4);
  ASSERT_EQ(constant_table->get().size(), 5);
  ASSERT_EQ(procedure_table->get().size(), 3);
  ASSERT_EQ(assign_table->get().size(), 5);
  ASSERT_EQ(read_table->get().size(), 4);
  ASSERT_EQ(if_table->get().size(), 3);
  ASSERT_EQ(while_table->get().size(), 1);
}