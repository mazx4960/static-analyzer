// Copyright 2022 CS3203 Team14. All rights reserved.


#include <gtest/gtest.h>

#include <vector>

#include "commons/entity.h"
#include "pkb/pkb.h"

TEST(PKBSaveTest, VariableTest) {
  std::vector<std::string> entity_names({"a", "b", "c", "d", "e", "f"});
  std::vector<Entity *> entities;
  int length = entity_names.size();

  for (int i = 0; i < length; i++) {
    entities.push_back(new VariableEntity(entity_names[i]));
  }

  PKB pkb;
  pkb.save(entities);
  ASSERT_EQ(pkb.getCount(), length);
}

TEST(PKBSaveTest, ConstTest) {
  std::vector<std::string> entity_names({"a", "b", "c", "d", "e", "f"});
  std::vector<Entity *> entities;
  int length = entity_names.size();

  for (int i = 0; i < length; i++) {
    entities.push_back(new ConstantEntity(entity_names[i]));
  }

  PKB pkb;
  pkb.save(entities);
  ASSERT_EQ(pkb.getCount(), length);
}

TEST(PKBSaveTest, ConstantAndVariableTest) {
  std::vector<std::string> v_entity_names({"a", "b", "c", "d"});
  std::vector<std::string> c_entity_names({"ee", "ff", "gg"});
  std::vector<Entity *> v_entities;
  std::vector<Entity *> c_entities;

  for (const auto &s : v_entity_names) {
    v_entities.push_back(new VariableEntity(s));
  }

  for (const auto &s : c_entity_names) {
    c_entities.push_back(new ConstantEntity(s));
  }

  PKB pkb;
  pkb.save(v_entities);
  pkb.save(c_entities);

  ASSERT_EQ(pkb.getCount(), c_entity_names.size() + v_entity_names.size());
}

TEST(PKBSaveTest, MultiSave) {
  std::vector<std::string> entity_names({"a", "b", "c", "d", "e", "f"});
  std::vector<Entity *> entities;
  int length = entity_names.size();

  for (int i = 0; i < length; i++) {
    entities.push_back(new VariableEntity(entity_names[i]));
  }

  PKB pkb;
  for (int i = 0; i < 100; i++) {
    pkb.save(entities);
  }
  ASSERT_EQ(pkb.getCount(), length);
}