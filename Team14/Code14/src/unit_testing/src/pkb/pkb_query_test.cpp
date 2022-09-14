// Copyright 2022 CS3203 Team14. All rights reserved.

#include "gtest/gtest.h"
#include "pkb/pkb.h"
#include "pkb/relationship/relationship_table.h"

TEST(QueryTest, FollowsRelationshipInversionFalse) {
  std::vector<std::pair<std::string, std::string>> follows = {
      {"1", "2"},
      {"2", "3"},
      {"3", "4"},
      {"8", "10"}
  };
  std::vector<Relationship *> relationships;
  for (auto & follow : follows) {
    relationships.push_back(
        new FollowsRelationship(
            new AssignStmtEntity(follow.first),
            new AssignStmtEntity(follow.second))
    );
  }
  PKB pkb;
  pkb.populate(relationships);
  auto result = pkb.getByRelationship(RsType::kFollows, new AssignStmtEntity("8"), false);
  std::unordered_set<Entity *> expected_result = {new AssignStmtEntity("10")};
  ASSERT_EQ((*result.begin())->GetValue(), (*expected_result.begin())->GetValue());
}

TEST(QueryTest, ParentTRelationshipInversionFalse) {
  std::vector<std::pair<std::string, std::string>> parent = {
      {"2", "1"},
      {"3", "2"},
      {"4", "3"},
      {"10", "8"}
  };
  std::vector<Relationship *> relationships;
  for (auto & p : parent) {
    relationships.push_back(
        new ParentRelationship(
            new WhileStmtEntity(p.first),
            new WhileStmtEntity(p.second))
    );
  }
  PKB pkb;
  pkb.populate(relationships);
  auto result = pkb.getByRelationship(RsType::kParentT, new WhileStmtEntity("4"), false);
  std::unordered_set<Entity *> expected_result = {new WhileStmtEntity("3"),
                                                  new IfStmtEntity("2"),
                                                  new WhileStmtEntity("1")};

  ASSERT_EQ(result.size(), expected_result.size());
}

TEST(QuerTest, ParentRelationshipInversionTrue) {
  PKB pkb;
  auto result = pkb.getByRelationship(RsType::kParent, new AssignStmtEntity("3"), true);
  std::unordered_set<Entity *> expected_result = {};
  ASSERT_EQ(result, expected_result);
}

TEST(QueryTest, ModifiesRelationshipInversionFalse) {
  std::vector<std::pair<std::string, std::string>> modifies = {
      {"1", "X"},
      {"2", "Y"},
      {"3", "Z"},
      {"3", "A"}
  };

  std::vector<Relationship *> relationships;
  for (auto & m: modifies) {
    relationships.push_back(
        new ModifiesRelationship(
            new AssignStmtEntity(m.first),
            new VariableEntity(m.second))
    );
  }
  PKB pkb;
  pkb.populate(relationships);
  auto query_result = pkb.getByRelationship(RsType::kModifies, new AssignStmtEntity("3"), false);
  std::unordered_set<std::string> v_result = {};
  for (auto *entity: query_result) {
    v_result.insert(entity->GetValue());
  }
  std::unordered_set<std::string> expected_result = {"A","Z"};
  ASSERT_EQ(v_result, expected_result);
}

TEST(QueryTest, ModifiesRelationshipInversionTrue) {
  std::vector<std::pair<std::string, std::string>> modifies = {
      {"1", "X"},
      {"2", "Y"},
      {"3", "Z"},
      {"3", "A"}
  };

  std::vector<Relationship *> relationships;
  for (auto & m: modifies) {
    relationships.push_back(
        new ModifiesRelationship(
            new AssignStmtEntity(m.first),
            new VariableEntity(m.second))
    );
  }
  PKB pkb;
  pkb.populate(relationships);
  auto result = pkb.getByRelationship(RsType::kModifies, new VariableEntity("X"), true);
  std::unordered_set<Entity *> expected_result = {new AssignStmtEntity("1")};
  ASSERT_EQ((*result.begin())->GetValue(), (*expected_result.begin())->GetValue());
}

TEST(QueryTest, UsesRelationshipInvalidQueryInversionFalse) {
  std::vector<std::pair<std::string, std::string>> modifies = {
      {"1", "X"},
      {"2", "Y"},
      {"3", "Z"},
      {"3", "A"}
  };

  std::vector<Relationship *> relationships;
  for (auto & m: modifies) {
    relationships.push_back(
        new UsesRelationship(
            new WhileStmtEntity(m.first),
            new IfStmtEntity(m.second))
    );
  }
  PKB pkb;
  pkb.populate(relationships);
  auto result = pkb.getByRelationship(RsType::kUses, new VariableEntity("X"), false);
  std::unordered_set<Entity *> expected_result = {};
  ASSERT_EQ(result, expected_result);
}