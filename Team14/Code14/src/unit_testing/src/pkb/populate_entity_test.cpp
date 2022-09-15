// Copyright 2022 CS3203 Team14. All rights reserved.

#include <gtest/gtest.h>

#include <vector>

#include "commons/entity.h"
#include "pkb/pkb.h"

TEST(PopulateTest, VariableTest) {
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

TEST(PopulateTest, ConstTest) {
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

TEST(PopulateTest, MixedEntityTest) {
  std::vector<Entity *> entities;

  std::vector<std::string> v_entity_names({"a", "b", "c", "d"});
  std::vector<std::string> c_entity_names({"ee", "ff", "gg"});
  for (const auto &s : v_entity_names) { entities.push_back(new VariableEntity(s)); }
  for (const auto &s : c_entity_names) { entities.push_back(new ConstantEntity(s)); }

  auto *entity_manager = new EntityManager();
  entity_manager->Populate(entities);
  auto *variable_table = entity_manager->GetTable(EntityType::kVariable);
  auto *constant_table = entity_manager->GetTable(EntityType::kConstant);
  ASSERT_EQ(variable_table->get().size(), 4);
  ASSERT_EQ(constant_table->get().size(), 3);
}
