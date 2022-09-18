// Copyright 2022 CS3203 Team14. All rights reserved.

#include <gtest/gtest.h>

#include <vector>

#include "commons/entity.h"
#include "pkb/pkb.h"

TEST(PopulateEntityTest, VariableTest) {
  std::vector<std::string> entity_names({"a", "b", "c", "d", "e", "f"});
  std::vector<Entity *> entities;
  int length = entity_names.size();
  entities.reserve(length);

  for (int i = 0; i < length; i++) { entities.push_back(new VariableEntity(entity_names[i])); }

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

  for (int i = 0; i < length; i++) { entities.push_back(new ConstantEntity(entity_names[i])); }

  auto *entity_manager = new EntityManager();
  entity_manager->Populate(entities);
  auto *entity_table = entity_manager->GetTable(EntityType::kConstant);
  ASSERT_EQ(entity_table->get().size(), length);
}

TEST(PopulateEntityTest, AssignTest) {
  std::vector<std::string> stmt_no({"1", "2", "3", "4", "5", "6"});
  std::vector<Entity *> entities;
  int length = stmt_no.size();
  entities.reserve(length);

  for (int i = 0; i < length; i++) { entities.push_back(new AssignStmtEntity(stmt_no[i])); }

  auto *entity_manager = new EntityManager();
  entity_manager->Populate(entities);
  auto *entity_table = entity_manager->GetTable(EntityType::kAssignStmt);
  ASSERT_EQ(entity_table->get().size(), length);
}

TEST(PopulateEntityTest, MultipleEntitiesTest) {
  std::vector<Entity *> entities;

  std::vector<std::string> v_entity_names({"a", "b", "c", "d"});
  std::vector<std::string> c_entity_names({"ee", "ff", "gg"});
  std::vector<std::string> a_stmt_no({"1", "2", "3", "4", "5"});
  for (const auto &s : v_entity_names) { entities.push_back(new VariableEntity(s)); }
  for (const auto &s : c_entity_names) { entities.push_back(new ConstantEntity(s)); }
  for (const auto &s : a_stmt_no) { entities.push_back(new AssignStmtEntity(s)); }

  auto *entity_manager = new EntityManager();
  entity_manager->Populate(entities);
  auto *variable_table = entity_manager->GetTable(EntityType::kVariable);
  auto *constant_table = entity_manager->GetTable(EntityType::kConstant);
  auto *assign_table = entity_manager->GetTable(EntityType::kAssignStmt);
  ASSERT_EQ(variable_table->get().size(), 4);
  ASSERT_EQ(constant_table->get().size(), 3);
  ASSERT_EQ(assign_table->get().size(), 5);
}

TEST(PopulateEntityTest, DuplicateEntitiesTest) {
  std::vector<std::string> entity_names({"a", "a", "a", "a", "a", "a"});
  std::vector<Entity *> entities;
  int length = entity_names.size();
  entities.reserve(length);

  for (int i = 0; i < length; i++) { entities.push_back(new VariableEntity(entity_names[i])); }

  auto *entity_manager = new EntityManager();
  entity_manager->Populate(entities);
  auto *entity_table = entity_manager->GetTable(EntityType::kVariable);
  ASSERT_EQ(entity_table->get().size(), 1);
}