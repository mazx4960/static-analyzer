// Copyright 2022 CS3203 Team14. All rights reserved.


#include <gtest/gtest.h>

#include <vector>

#include "commons/entity.h"
#include "pkb/pkb.h"

TEST(PKBPopulateTest, CallStatementTest) {
  std::vector<int> stmt_no({1, 2, 3, 4, 5, 6, 7});
  std::vector<Entity *> entities;
  int length = stmt_no.size();
  entities.reserve(length);

  for (int i = 0; i < length; i++) { entities.push_back(new CallEntity(stmt_no[i])); }

  PKB pkb;
  pkb.populate(entities);
  ASSERT_EQ(pkb.getCount(), length);
}

TEST(PKBPopulateTest, CallStatementAndVariableTest) {
  std::vector<int> stmt_no({1, 2, 3, 4, 5, 6, 7});
  std::vector<Entity *> call_entities;
  int call_length = stmt_no.size();
  call_entities.reserve(call_length);

  std::vector<std::string> var_names({"a", "b", "c", "d", "e", "f"});
  std::vector<Entity *> var_entities;
  int var_length = var_names.size();
  var_entities.reserve(var_length);

  for (int i = 0; i < call_length; i++) { call_entities.push_back(new CallEntity(stmt_no[i])); }

  for (int j = 0; j < var_length; j++) { var_entities.push_back(new VariableEntity(var_names[j])); }

  PKB pkb;
  int length = call_length + var_length;
  pkb.populate(var_entities);
  pkb.populate(call_entities);
  ASSERT_EQ(pkb.getCount(), length);
}


TEST(PKBPopulateTest, VariableTest) {
  std::vector<std::string> entity_names({"a", "b", "c", "d", "e", "f"});
  std::vector<Entity *> entities;
  int length = entity_names.size();
  entities.reserve(length);

  for (int i = 0; i < length; i++) {
    entities.push_back(new VariableEntity(entity_names[i]));
  }

  PKB pkb;
  pkb.populate(entities);
  ASSERT_EQ(pkb.getCount(), length);
}

TEST(PKBPopulateTest, ConstTest) {
  std::vector<std::string> entity_names({"a", "b", "c", "d", "e", "f"});
  std::vector<Entity *> entities;
  int length = entity_names.size();
  entities.reserve(length);

  for (int i = 0; i < length; i++) {
    entities.push_back(new ConstantEntity(entity_names[i]));
  }

  PKB pkb;
  pkb.populate(entities);
  ASSERT_EQ(pkb.getCount(), length);
}

TEST(PKBPopulateTest, ConstantAndVariableTest) {
  std::vector<std::string> v_entity_names({"a", "b", "c", "d"});
  std::vector<std::string> c_entity_names({"ee", "ff", "gg"});
  std::vector<Entity *> v_entities;
  std::vector<Entity *> c_entities;
  v_entities.reserve(v_entity_names.size());
  c_entities.reserve(c_entity_names.size());

  for (const auto &s : v_entity_names) {
    v_entities.push_back(new VariableEntity(s));
  }

  for (const auto &s : c_entity_names) {
    c_entities.push_back(new ConstantEntity(s));
  }

  PKB pkb;
  pkb.populate(v_entities);
  pkb.populate(c_entities);

  ASSERT_EQ(pkb.getCount(), c_entity_names.size() + v_entity_names.size());
}

TEST(PKBPopulateTest, DuplicatePopulation) {
  std::vector<std::string> entity_names({"a", "b", "c", "d", "e", "f"});
  std::vector<Entity *> entities;
  int length = entity_names.size();
  entities.reserve(length);

  for (int i = 0; i < length; i++) {
    entities.push_back(new VariableEntity(entity_names[i]));
  }

  PKB pkb;
  for (int i = 0; i < 100; i++) {
    pkb.populate(entities);
  }
  ASSERT_EQ(pkb.getCount(), length);
}