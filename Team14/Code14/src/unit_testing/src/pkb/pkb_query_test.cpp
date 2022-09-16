// Copyright 2022 CS3203 Team14. All rights reserved.

#include "gtest/gtest.h"
#include "pkb/pkb.h"
#include "pkb/relationship/relationship_table.h"

TEST(QueryTest, FollowsRelationship) {
  std::vector<Relationship *> relationships = {
      new FollowsRelationship(new AssignStmtEntity("1"), new AssignStmtEntity("2")),
      new FollowsRelationship(new AssignStmtEntity("2"), new AssignStmtEntity("3")),
      new FollowsRelationship(new AssignStmtEntity("3"), new AssignStmtEntity("4")),
      new FollowsRelationship(new AssignStmtEntity("8"), new AssignStmtEntity("10")),
  };
  PKB pkb;
  pkb.populate(relationships);
  auto result_inverse_false = pkb.getByRelationship(RsType::kFollows, new AssignStmtEntity("8"), false);
  auto result_inverse_true = pkb.getByRelationship(RsType::kFollows, new AssignStmtEntity("10"), true);
  auto result_traverse_inverse_false = pkb.getByRelationship(RsType::kFollowsT, new AssignStmtEntity("1"), false);
  auto result_traverse_inverse_true = pkb.getByRelationship(RsType::kFollowsT, new AssignStmtEntity("4"), true);
  auto result_no_preceding_statement = pkb.getByRelationship(RsType::kFollows, new AssignStmtEntity("1"), true);
  auto result_no_subsequent_statement = pkb.getByRelationship(RsType::kFollows, new AssignStmtEntity("10"), false);
  std::unordered_set<Entity *, EntityHashFunction, EntityPointerEquality> expected_result_inverse_false = {new AssignStmtEntity("10")};
  std::unordered_set<Entity *, EntityHashFunction, EntityPointerEquality> expected_result_inverse_true = {new AssignStmtEntity("8")};
  std::unordered_set<Entity *, EntityHashFunction, EntityPointerEquality> expected_result_traverse_inverse_false = {
      new AssignStmtEntity("2"),
      new AssignStmtEntity("3"),
      new AssignStmtEntity("4")};
  std::unordered_set<Entity *, EntityHashFunction, EntityPointerEquality> expected_result_traverse_inverse_true = {
      new AssignStmtEntity("1"),
      new AssignStmtEntity("2"),
      new AssignStmtEntity("3")};
  std::unordered_set<Entity *, EntityHashFunction, EntityPointerEquality> expected_empty = {};
  ASSERT_EQ(result_inverse_false, expected_result_inverse_false);
  ASSERT_EQ(result_inverse_true, expected_result_inverse_true);
  ASSERT_EQ(result_traverse_inverse_false, expected_result_traverse_inverse_false);
  ASSERT_EQ(result_traverse_inverse_true, expected_result_traverse_inverse_true);
  ASSERT_EQ(result_no_preceding_statement, expected_empty);
  ASSERT_EQ(result_no_subsequent_statement, expected_empty);
}
TEST(QueryTest, ParentRelationship) {
  std::vector<Relationship *> relationships = {
      new ParentRelationship(new ProcedureEntity("main"), new WhileStmtEntity("12")),
      new ParentRelationship(new WhileStmtEntity("1"), new IfStmtEntity("2")),
      new ParentRelationship(new IfStmtEntity("2"), new WhileStmtEntity("3")),
      new ParentRelationship(new WhileStmtEntity("3"), new AssignStmtEntity("4")),
      new ParentRelationship(new ProcedureEntity("Megatron"), new AssignStmtEntity("10")),
  };
  PKB pkb;
  pkb.populate(relationships);
  auto result_inverse_false = pkb.getByRelationship(RsType::kParent, new ProcedureEntity("Megatron"), false);
  auto result_inverse_true = pkb.getByRelationship(RsType::kParent, new AssignStmtEntity("10"), true);
  auto result_traverse_inverse_false = pkb.getByRelationship(RsType::kParentT, new WhileStmtEntity("1"), false);
  auto result_traverse_inverse_true = pkb.getByRelationship(RsType::kParentT, new AssignStmtEntity("4"), true);
  auto result_no_parent = pkb.getByRelationship(RsType::kParentT, new ProcedureEntity("main"), true);
  auto result_no_child = pkb.getByRelationship(RsType::kParent, new AssignStmtEntity("10"), false);
  std::unordered_set<Entity *, EntityHashFunction, EntityPointerEquality> expected_result_inverse_false = {new AssignStmtEntity("10")};
  std::unordered_set<Entity *, EntityHashFunction, EntityPointerEquality> expected_result_inverse_true = {new ProcedureEntity("Megatron")};
  std::unordered_set<Entity *, EntityHashFunction, EntityPointerEquality> expected_result_traverse_inverse_false = {
      new IfStmtEntity("2"),
      new WhileStmtEntity("3"),
      new AssignStmtEntity("4")};
  std::unordered_set<Entity *, EntityHashFunction, EntityPointerEquality> expected_result_traverse_inverse_true = {
      new WhileStmtEntity("3"),
      new IfStmtEntity("2"),
      new WhileStmtEntity("1")};
  std::unordered_set<Entity *, EntityHashFunction, EntityPointerEquality> expected_result_empty = {};
  ASSERT_EQ(result_inverse_false, expected_result_inverse_false);
  ASSERT_EQ(result_inverse_true, expected_result_inverse_true);
  ASSERT_EQ(result_traverse_inverse_false, expected_result_traverse_inverse_false);
  ASSERT_EQ(result_traverse_inverse_true, expected_result_traverse_inverse_true);
  ASSERT_EQ(result_no_parent, expected_result_empty);
  ASSERT_EQ(result_no_child, expected_result_empty);
}
TEST(QueryTest, ModifiesRelationship) {
  std::vector<Relationship *> relationships = {
      new ModifiesRelationship(new AssignStmtEntity("1"), new VariableEntity("X")),
      new ModifiesRelationship(new AssignStmtEntity("2"), new VariableEntity("X")),
      new ModifiesRelationship(new ReadStmtEntity("3"), new VariableEntity("Y")),
      new ModifiesRelationship(new ProcedureEntity("main"), new VariableEntity("Z")),
  };
  PKB pkb;
  pkb.populate(relationships);
  auto result_single_inverse_false = pkb.getByRelationship(RsType::kModifies, new ReadStmtEntity("3"), false);
  auto result_single_inverse_true = pkb.getByRelationship(RsType::kModifies, new VariableEntity("Z"), true);
  auto result_invalid_procedure = pkb.getByRelationship(RsType::kModifies, new ProcedureEntity("Megatron"), false);
  auto result_invalid_variable = pkb.getByRelationship(RsType::kModifies, new VariableEntity("A"), false);
  auto result_multiple_inverse_true = pkb.getByRelationship(RsType::kModifies, new VariableEntity("X"), true);
  std::unordered_set<Entity *, EntityHashFunction, EntityPointerEquality> expected_result_single_inverse_false = {new VariableEntity("Y")};
  std::unordered_set<Entity *, EntityHashFunction, EntityPointerEquality> expected_result_single_inverse_true = {new ProcedureEntity("main")};
  std::unordered_set<Entity *, EntityHashFunction, EntityPointerEquality> expected_result_empty = {};
  std::unordered_set<Entity *, EntityHashFunction, EntityPointerEquality> expected_result_multiple_inverse_true = {
      new AssignStmtEntity("1"),
      new AssignStmtEntity("2")};
  ASSERT_EQ(result_single_inverse_false, expected_result_single_inverse_false);
  ASSERT_EQ(result_single_inverse_true, expected_result_single_inverse_true);
  ASSERT_EQ(result_invalid_procedure, expected_result_empty);
  ASSERT_EQ(result_invalid_variable, expected_result_empty);
  ASSERT_EQ(result_multiple_inverse_true, expected_result_multiple_inverse_true);
}
TEST(QueryTest, UsesRelationship) {
  std::vector<Relationship *> relationships = {
      new UsesRelationship(new AssignStmtEntity("1"), new VariableEntity("X")),
      new UsesRelationship(new AssignStmtEntity("1"), new VariableEntity("A")),
      new UsesRelationship(new AssignStmtEntity("2"), new VariableEntity("X")),
      new UsesRelationship(new ReadStmtEntity("3"), new VariableEntity("Y")),
      new UsesRelationship(new ProcedureEntity("main"), new VariableEntity("Z")),
  };
  PKB pkb;
  pkb.populate(relationships);
  auto result_single_inverse_false = pkb.getByRelationship(RsType::kUses, new ReadStmtEntity("3"), false);
  auto result_single_inverse_true = pkb.getByRelationship(RsType::kUses, new VariableEntity("Z"), true);
  auto result_invalid_procedure = pkb.getByRelationship(RsType::kUses, new ProcedureEntity("Megatron"), false);
  auto result_invalid_variable = pkb.getByRelationship(RsType::kUses, new VariableEntity("east"), true);
  auto result_multiple_inverse_false = pkb.getByRelationship(RsType::kUses, new AssignStmtEntity("1"), false);
  auto result_multiple_inverse_true = pkb.getByRelationship(RsType::kUses, new VariableEntity("X"), true);
  std::unordered_set<Entity *, EntityHashFunction, EntityPointerEquality> expected_result_single_inverse_false = {new VariableEntity("Y")};
  std::unordered_set<Entity *, EntityHashFunction, EntityPointerEquality> expected_result_single_inverse_true = {new ProcedureEntity("main")};
  std::unordered_set<Entity *, EntityHashFunction, EntityPointerEquality> expected_result_multiple_inverse_false = {
      new VariableEntity("X"),
      new VariableEntity("A")};
  std::unordered_set<Entity *, EntityHashFunction, EntityPointerEquality> expected_result_multiple_inverse_true = {
      new AssignStmtEntity("1"),
      new AssignStmtEntity("2")};
  std::unordered_set<Entity *, EntityHashFunction, EntityPointerEquality> expected_result_empty = {};
  ASSERT_EQ(result_single_inverse_false, expected_result_single_inverse_false);
  ASSERT_EQ(result_single_inverse_true, expected_result_single_inverse_true);
  ASSERT_EQ(result_invalid_procedure, expected_result_empty);
  ASSERT_EQ(result_invalid_variable, expected_result_empty);
  ASSERT_EQ(result_multiple_inverse_false, expected_result_multiple_inverse_false);
  ASSERT_EQ(result_multiple_inverse_true, expected_result_multiple_inverse_true);
}
