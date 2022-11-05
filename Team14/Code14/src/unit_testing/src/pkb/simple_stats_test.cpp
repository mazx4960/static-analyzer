// Copyright 2022 CS3203 Team14. All rights reserved.

#include "gtest/gtest.h"
#include "pkb/pkb.h"
#include "pkb/entity/entity_manager.h"

TEST(SimpleStats, SingleEntity) {
  std::vector<Entity *> entities = {
    new VariableEntity("x"), new VariableEntity("y"), new VariableEntity("z"),
  };
  PKB pkb;
  pkb.populate(entities);
  std::unordered_map<EntityType, int> map = pkb.getSimpleStats();

  ASSERT_EQ(map.find(EntityType::kVariable)->second, 3);
  ASSERT_EQ(map.find(EntityType::kProcedure)->second, 0);
}

TEST(SimpleStats, MultipleEntities) {
  std::vector<Entity *> entities = {
      new VariableEntity("x"), new VariableEntity("y"), new VariableEntity("z"),
      new ConstantEntity("1"), new ConstantEntity("2"), new ConstantEntity("3"),
      new AssignStmtEntity("1"), new AssignStmtEntity("2"), new AssignStmtEntity("3"),
      new AssignStmtEntity("4"), new WhileStmtEntity("5"), new IfStmtEntity("6"),
  };
  PKB pkb;
  pkb.populate(entities);
  std::unordered_map<EntityType, int> map = pkb.getSimpleStats();

  ASSERT_EQ(map.find(EntityType::kVariable)->second, 3);
  ASSERT_EQ(map.find(EntityType::kConstant)->second, 3);
  ASSERT_EQ(map.find(EntityType::kAssignStmt)->second, 4);
  ASSERT_EQ(map.find(EntityType::kProcedure)->second, 0);
  ASSERT_EQ(map.find(EntityType::kStatement)->second, 6);
}