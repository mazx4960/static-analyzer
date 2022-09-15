// Copyright 2022 CS3203 Team14. All rights reserved.

#include "gtest/gtest.h"
#include "pkb/pkb.h"
#include "pkb/relationship/relationship_table.h"

TEST(QueryTest, FollowsRelationshipInversionFalse) {
  std::vector<Relationship *> relationships = {
      new FollowsRelationship(new AssignStmtEntity("1"), new AssignStmtEntity("2")),
      new FollowsRelationship(new AssignStmtEntity("2"), new AssignStmtEntity("3")),
      new FollowsRelationship(new AssignStmtEntity("3"), new AssignStmtEntity("4")),
      new FollowsRelationship(new AssignStmtEntity("8"), new AssignStmtEntity("10")),
  };
  PKB pkb;
  pkb.populate(relationships);
  auto result = pkb.getByRelationship(RsType::kFollows, new AssignStmtEntity("8"), false);
  std::unordered_set<Entity *, EntityHashFunction, EntityPointerEquality> expected_result = {new AssignStmtEntity("10")};
  ASSERT_EQ(result, expected_result);
}
TEST(QueryTest, FollowsRelationshipInversionTrue) {
  std::vector<Relationship *> relationships = {
      new FollowsRelationship(new AssignStmtEntity("1"), new AssignStmtEntity("2")),
      new FollowsRelationship(new AssignStmtEntity("2"), new AssignStmtEntity("3")),
      new FollowsRelationship(new AssignStmtEntity("3"), new AssignStmtEntity("4")),
      new FollowsRelationship(new AssignStmtEntity("8"), new AssignStmtEntity("10")),
  };
  PKB pkb;
  pkb.populate(relationships);
  auto result = pkb.getByRelationship(RsType::kFollows, new AssignStmtEntity("10"), true);
  std::unordered_set<Entity *, EntityHashFunction, EntityPointerEquality> expected_result = {new AssignStmtEntity("8")};
  ASSERT_EQ(result, expected_result);
}
TEST(QueryTest, FollowsTRelationshipInversionFalse) {
  std::vector<Relationship *> relationships = {
      new FollowsRelationship(new AssignStmtEntity("1"), new AssignStmtEntity("2")),
      new FollowsRelationship(new AssignStmtEntity("2"), new AssignStmtEntity("3")),
      new FollowsRelationship(new AssignStmtEntity("3"), new AssignStmtEntity("4")),
      new FollowsRelationship(new AssignStmtEntity("8"), new AssignStmtEntity("10")),
  };
  PKB pkb;
  pkb.populate(relationships);
  auto result = pkb.getByRelationship(RsType::kFollowsT, new AssignStmtEntity("1"), false);
  std::unordered_set<Entity *, EntityHashFunction, EntityPointerEquality> expected_result = {
      new AssignStmtEntity("2"),
      new AssignStmtEntity("3"),
      new AssignStmtEntity("4")
  };
  ASSERT_EQ(result, expected_result);
}
TEST(QueryTest, FollowsTRelationshipInversionTrue) {
  std::vector<Relationship *> relationships = {
      new FollowsRelationship(new AssignStmtEntity("1"), new AssignStmtEntity("2")),
      new FollowsRelationship(new AssignStmtEntity("2"), new AssignStmtEntity("3")),
      new FollowsRelationship(new AssignStmtEntity("3"), new AssignStmtEntity("4")),
      new FollowsRelationship(new AssignStmtEntity("8"), new AssignStmtEntity("10")),
  };
  PKB pkb;
  pkb.populate(relationships);
  auto result = pkb.getByRelationship(RsType::kFollowsT, new AssignStmtEntity("4"), true);
  std::unordered_set<Entity *, EntityHashFunction, EntityPointerEquality> expected_result = {
      new AssignStmtEntity("1"),
      new AssignStmtEntity("2"),
      new AssignStmtEntity("3")
  };
  ASSERT_EQ(result, expected_result);
}
TEST(QueryTest, ParentRelationshipInversionFalse) {
  std::vector<Relationship *> relationships = {
      new ParentRelationship(new WhileStmtEntity("1"), new IfStmtEntity("2")),
      new ParentRelationship(new IfStmtEntity("2"), new WhileStmtEntity("3")),
      new ParentRelationship(new WhileStmtEntity("3"), new AssignStmtEntity("4")),
      new ParentRelationship(new AssignStmtEntity("8"), new AssignStmtEntity("10")),
  };
  PKB pkb;
  pkb.populate(relationships);
  auto result = pkb.getByRelationship(RsType::kParent, new AssignStmtEntity("8"), false);
  std::unordered_set<Entity *, EntityHashFunction, EntityPointerEquality> expected_result = {new AssignStmtEntity("10")};
  ASSERT_EQ(result, expected_result);
}
TEST(QueryTest, ParentRelationshipInversionTrue) {
  std::vector<Relationship *> relationships = {
      new ParentRelationship(new WhileStmtEntity("1"), new IfStmtEntity("2")),
      new ParentRelationship(new IfStmtEntity("2"), new WhileStmtEntity("3")),
      new ParentRelationship(new WhileStmtEntity("3"), new AssignStmtEntity("4")),
      new ParentRelationship(new AssignStmtEntity("8"), new AssignStmtEntity("10")),
  };
  PKB pkb;
  pkb.populate(relationships);
  auto result = pkb.getByRelationship(RsType::kParent, new AssignStmtEntity("10"), true);
  std::unordered_set<Entity *, EntityHashFunction, EntityPointerEquality> expected_result = {new AssignStmtEntity("8")};
  ASSERT_EQ(result, expected_result);
}
TEST(QueryTest, ParentTRelationshipInversionFalse) {
  std::vector<Relationship *> relationships = {
      new ParentRelationship(new WhileStmtEntity("1"), new IfStmtEntity("2")),
      new ParentRelationship(new IfStmtEntity("2"), new WhileStmtEntity("3")),
      new ParentRelationship(new WhileStmtEntity("3"), new AssignStmtEntity("4")),
      new ParentRelationship(new AssignStmtEntity("8"), new AssignStmtEntity("10")),
  };
  PKB pkb;
  pkb.populate(relationships);
  auto result = pkb.getByRelationship(RsType::kParentT, new WhileStmtEntity("1"), false);
  std::unordered_set<Entity *, EntityHashFunction, EntityPointerEquality> expected_result = {
      new IfStmtEntity("2"),
      new WhileStmtEntity("3"),
      new AssignStmtEntity("4")};
  ASSERT_EQ(result, expected_result);
}
TEST(QueryTest, ParentTRelationshipInversionTrue) {
  std::vector<Relationship *> relationships = {
      new ParentRelationship(new WhileStmtEntity("1"), new IfStmtEntity("2")),
      new ParentRelationship(new IfStmtEntity("2"), new WhileStmtEntity("3")),
      new ParentRelationship(new WhileStmtEntity("3"), new AssignStmtEntity("4")),
      new ParentRelationship(new AssignStmtEntity("8"), new AssignStmtEntity("10")),
  };
  PKB pkb;
  pkb.populate(relationships);
  auto result = pkb.getByRelationship(RsType::kParentT, new AssignStmtEntity("4"), true);
  std::unordered_set<Entity *, EntityHashFunction, EntityPointerEquality> expected_result = {
      new WhileStmtEntity("3"),
      new IfStmtEntity("2"),
      new WhileStmtEntity("1")};
  ASSERT_EQ(result, expected_result);
}
TEST(QueryTest, ModifiesRelationshipInversionFalse) {
  std::vector<Relationship *> relationships = {
      new ModifiesRelationship(new AssignStmtEntity("1"), new VariableEntity("X")),
      new ModifiesRelationship(new AssignStmtEntity("2"), new VariableEntity("X")),
      new ModifiesRelationship(new ReadStmtEntity("3"), new VariableEntity("Y")),
      new ModifiesRelationship(new ProcedureEntity("main"), new VariableEntity("Z")),
  };
  PKB pkb;
  pkb.populate(relationships);
  auto result = pkb.getByRelationship(RsType::kModifies, new AssignStmtEntity("1"), false);
  std::unordered_set<Entity *, EntityHashFunction, EntityPointerEquality> expected_result = {new VariableEntity("X")};
  ASSERT_EQ(result, expected_result);
}
TEST(QueryTest, ModifiesRelationshipInversionTrue) {
  std::vector<Relationship *> relationships = {
      new ModifiesRelationship(new AssignStmtEntity("1"), new VariableEntity("X")),
      new ModifiesRelationship(new AssignStmtEntity("2"), new VariableEntity("X")),
      new ModifiesRelationship(new ReadStmtEntity("3"), new VariableEntity("Y")),
      new ModifiesRelationship(new ProcedureEntity("main"), new VariableEntity("Z")),
  };
  PKB pkb;
  pkb.populate(relationships);
  auto result = pkb.getByRelationship(RsType::kModifies, new VariableEntity("X"), true);
  std::unordered_set<Entity *, EntityHashFunction, EntityPointerEquality> expected_result = {
      new AssignStmtEntity("1"),
      new AssignStmtEntity("2")};
  ASSERT_EQ(result, expected_result);
}
TEST(QueryTest, UsesRelationshipInversionFalse) {
  std::vector<Relationship *> relationships = {
      new UsesRelationship(new AssignStmtEntity("1"), new VariableEntity("X")),
      new UsesRelationship(new AssignStmtEntity("2"), new VariableEntity("X")),
      new UsesRelationship(new ReadStmtEntity("3"), new VariableEntity("Y")),
      new UsesRelationship(new ProcedureEntity("main"), new VariableEntity("Z")),
  };
  PKB pkb;
  pkb.populate(relationships);
  auto result = pkb.getByRelationship(RsType::kUses, new ReadStmtEntity("3"), false);
  std::unordered_set<Entity *, EntityHashFunction, EntityPointerEquality> expected_result = {new VariableEntity("Y")};
  ASSERT_EQ(result, expected_result);
}
TEST(QueryTest, UsesRelationshipInversionTrue) {
  std::vector<Relationship *> relationships = {
      new UsesRelationship(new AssignStmtEntity("1"), new VariableEntity("X")),
      new UsesRelationship(new AssignStmtEntity("2"), new VariableEntity("X")),
      new UsesRelationship(new ReadStmtEntity("3"), new VariableEntity("Y")),
      new UsesRelationship(new ProcedureEntity("main"), new VariableEntity("Z")),
  };
  PKB pkb;
  pkb.populate(relationships);
  auto result = pkb.getByRelationship(RsType::kUses, new VariableEntity("X"), true);
  std::unordered_set<Entity *, EntityHashFunction, EntityPointerEquality> expected_result = {
      new AssignStmtEntity("1"),
      new AssignStmtEntity("2")};
  ASSERT_EQ(result, expected_result);
}
