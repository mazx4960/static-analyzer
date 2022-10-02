// Copyright 2022 CS3203 Team14. All rights reserved.

#include "gtest/gtest.h"
#include "pkb/pkb.h"
#include "pkb/relationship/relationship_table.h"
#include "pkb_test_helper.h"

using EntityPointerUnorderedSet = std::unordered_set<Entity *, EntityHashFunction, EntityPointerEquality>;

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
  auto result_traverse_inverse_false = pkb.getByRelationship(RsType::kFollowsAll, new AssignStmtEntity("1"), false);
  auto result_traverse_inverse_true = pkb.getByRelationship(RsType::kFollowsAll, new AssignStmtEntity("4"), true);
  auto result_no_preceding_statement = pkb.getByRelationship(RsType::kFollows, new AssignStmtEntity("1"), true);
  auto result_no_subsequent_statement = pkb.getByRelationship(RsType::kFollows, new AssignStmtEntity("10"), false);

  EntityPointerUnorderedSet expected_result_inverse_false = {new AssignStmtEntity("10")};
  EntityPointerUnorderedSet expected_result_inverse_true = {new AssignStmtEntity("8")};
  EntityPointerUnorderedSet expected_result_traverse_inverse_false = {
      new AssignStmtEntity("2"), new AssignStmtEntity("3"), new AssignStmtEntity("4")};
  EntityPointerUnorderedSet expected_result_traverse_inverse_true = {
      new AssignStmtEntity("1"), new AssignStmtEntity("2"), new AssignStmtEntity("3")};
  EntityPointerUnorderedSet expected_empty = {};

  ASSERT_TRUE(PKBTestHelper::set_compare(result_inverse_false, expected_result_inverse_false));
  ASSERT_TRUE(PKBTestHelper::set_compare(result_inverse_true, expected_result_inverse_true));
  ASSERT_TRUE(PKBTestHelper::set_compare(result_traverse_inverse_false, expected_result_traverse_inverse_false));
  ASSERT_TRUE(PKBTestHelper::set_compare(result_traverse_inverse_true, expected_result_traverse_inverse_true));
  ASSERT_TRUE(PKBTestHelper::set_compare(result_no_preceding_statement, expected_empty));
  ASSERT_TRUE(PKBTestHelper::set_compare(result_no_subsequent_statement, expected_empty));
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
  auto result_traverse_inverse_false = pkb.getByRelationship(RsType::kParentAll, new WhileStmtEntity("1"), false);
  auto result_traverse_inverse_true = pkb.getByRelationship(RsType::kParentAll, new AssignStmtEntity("4"), true);
  auto result_no_parent = pkb.getByRelationship(RsType::kParentAll, new ProcedureEntity("main"), true);
  auto result_no_child = pkb.getByRelationship(RsType::kParent, new AssignStmtEntity("10"), false);

  EntityPointerUnorderedSet expected_result_inverse_false = {new AssignStmtEntity("10")};
  EntityPointerUnorderedSet expected_result_inverse_true = {new ProcedureEntity("Megatron")};
  EntityPointerUnorderedSet expected_result_traverse_inverse_false = {new IfStmtEntity("2"), new WhileStmtEntity("3"),
                                                                      new AssignStmtEntity("4")};
  EntityPointerUnorderedSet expected_result_traverse_inverse_true = {new WhileStmtEntity("3"), new IfStmtEntity("2"),
                                                                     new WhileStmtEntity("1")};
  EntityPointerUnorderedSet expected_result_empty = {};

  ASSERT_TRUE(PKBTestHelper::set_compare(result_inverse_false, expected_result_inverse_false));
  ASSERT_TRUE(PKBTestHelper::set_compare(result_inverse_true, expected_result_inverse_true));
  ASSERT_TRUE(PKBTestHelper::set_compare(result_traverse_inverse_false, expected_result_traverse_inverse_false));
  ASSERT_TRUE(PKBTestHelper::set_compare(result_traverse_inverse_true, expected_result_traverse_inverse_true));
  ASSERT_TRUE(PKBTestHelper::set_compare(result_no_parent, expected_result_empty));
  ASSERT_TRUE(PKBTestHelper::set_compare(result_no_child, expected_result_empty));
}

TEST(QueryTest, ModifiesRelationship) {
  std::vector<Relationship *> relationships = {
      new ParentRelationship(new ProcedureEntity("main"), new AssignStmtEntity("1")),
      new ParentRelationship(new ProcedureEntity("main"), new WhileStmtEntity("2")),
      new ParentRelationship(new WhileStmtEntity("2"), new AssignStmtEntity("3")),
      new ParentRelationship(new ProcedureEntity("main"), new CallStmtEntity("4")),
      new ParentRelationship(new ProcedureEntity("main"), new ReadStmtEntity("5")),
      new ParentRelationship(new ProcedureEntity("main"), new IfStmtEntity("6")),
      new ParentRelationship(new IfStmtEntity("6"), new AssignStmtEntity("7")),
      new ParentRelationship(new IfStmtEntity("6"), new AssignStmtEntity("8")),
      new ParentRelationship(new ProcedureEntity("main"), new AssignStmtEntity("9")),
      new ModifiesRelationship(new AssignStmtEntity("1"), new VariableEntity("X")),
      new ModifiesRelationship(new AssignStmtEntity("3"), new VariableEntity("Y")),
      new ModifiesRelationship(new ReadStmtEntity("5"), new VariableEntity("A")),
      new ModifiesRelationship(new AssignStmtEntity("7"), new VariableEntity("X")),
      new ModifiesRelationship(new AssignStmtEntity("8"), new VariableEntity("Z")),
      new ModifiesRelationship(new AssignStmtEntity("9"), new VariableEntity("B")),
      new CallsRelationship(new CallStmtEntity("10"), new ProcedureEntity("main")),
  };

  PKB pkb;
  pkb.populate(relationships);

  auto result_single_inverse_false = pkb.getByRelationship(RsType::kModifies, new ReadStmtEntity("5"), false);
  auto result_single_inverse_true = pkb.getByRelationship(RsType::kModifies, new VariableEntity("B"), true);
  auto result_invalid_procedure = pkb.getByRelationship(RsType::kModifies, new ProcedureEntity("Megatron"), false);
  auto result_invalid_variable = pkb.getByRelationship(RsType::kModifies, new VariableEntity("C"), false);
  auto result_multiple_inverse_true = pkb.getByRelationship(RsType::kModifies, new VariableEntity("X"), true);
  auto result_inference_from_while = pkb.getByRelationship(RsType::kModifies, new WhileStmtEntity("2"), false);
  auto result_inference_from_if = pkb.getByRelationship(RsType::kModifies, new IfStmtEntity("6"), false);
  auto result_inference_from_procedure = pkb.getByRelationship(RsType::kModifies, new ProcedureEntity("main"), false);
  auto result_inference_from_call_stmt = pkb.getByRelationship(RsType::kModifies, new CallStmtEntity("10"), false);
  auto result_inference_from_variable = pkb.getByRelationship(RsType::kModifies, new VariableEntity("X"), true);

  EntityPointerUnorderedSet expected_result_single_inverse_false = {new VariableEntity("A")};
  EntityPointerUnorderedSet expected_result_single_inverse_true = {new AssignStmtEntity("9")};
  EntityPointerUnorderedSet expected_result_multiple_inverse_true = {new AssignStmtEntity("1"),
                                                                     new AssignStmtEntity("7")};
  EntityPointerUnorderedSet expected_result_inference_from_while = {new VariableEntity("Y")};
  EntityPointerUnorderedSet expected_result_inference_from_if = {new VariableEntity("X"),
                                                                 new VariableEntity("Z")};
  EntityPointerUnorderedSet expected_result_all_variable_in_procedure = {new VariableEntity("A"),
                                                                        new VariableEntity("B"),
                                                                        new VariableEntity("X"),
                                                                        new VariableEntity("Y"),
                                                                        new VariableEntity("Z")};
  EntityPointerUnorderedSet expected_result_inference_from_variable = {new AssignStmtEntity("1"),
                                                                       new AssignStmtEntity("7")};
  EntityPointerUnorderedSet expected_result_empty = {};

  ASSERT_TRUE(PKBTestHelper::set_compare(result_single_inverse_false, expected_result_single_inverse_false));
  ASSERT_TRUE(PKBTestHelper::set_compare(result_single_inverse_true, expected_result_single_inverse_true));
  ASSERT_TRUE(PKBTestHelper::set_compare(result_invalid_procedure, expected_result_empty));
  ASSERT_TRUE(PKBTestHelper::set_compare(result_invalid_variable, expected_result_empty));
  ASSERT_TRUE(PKBTestHelper::set_compare(result_multiple_inverse_true, expected_result_multiple_inverse_true));
  ASSERT_TRUE(PKBTestHelper::set_compare(result_inference_from_while, expected_result_inference_from_while));
  ASSERT_TRUE(PKBTestHelper::set_compare(result_inference_from_if, expected_result_inference_from_if));
  ASSERT_TRUE(PKBTestHelper::set_compare(result_inference_from_procedure, expected_result_all_variable_in_procedure));
  ASSERT_TRUE(PKBTestHelper::set_compare(result_inference_from_call_stmt, expected_result_all_variable_in_procedure));
  ASSERT_TRUE(PKBTestHelper::set_compare(result_inference_from_variable, expected_result_inference_from_variable));
}

TEST(QueryTest, UsesRelationship) {
  std::vector<Relationship *> relationships = {
      new ParentRelationship(new ProcedureEntity("main"), new AssignStmtEntity("1")),
      new ParentRelationship(new ProcedureEntity("main"), new WhileStmtEntity("2")),
      new ParentRelationship(new WhileStmtEntity("2"), new AssignStmtEntity("3")),
      new ParentRelationship(new ProcedureEntity("main"), new CallStmtEntity("4")),
      new ParentRelationship(new ProcedureEntity("main"), new PrintStmtEntity("5")),
      new ParentRelationship(new ProcedureEntity("main"), new IfStmtEntity("6")),
      new ParentRelationship(new IfStmtEntity("6"), new AssignStmtEntity("7")),
      new ParentRelationship(new IfStmtEntity("6"), new AssignStmtEntity("8")),
      new ParentRelationship(new ProcedureEntity("main"), new AssignStmtEntity("9")),
      new UsesRelationship(new AssignStmtEntity("1"), new VariableEntity("X")),
      new UsesRelationship(new AssignStmtEntity("1"), new VariableEntity("A")),
      new UsesRelationship(new AssignStmtEntity("3"), new VariableEntity("Y")),
      new UsesRelationship(new PrintStmtEntity("5"), new VariableEntity("A")),
      new UsesRelationship(new AssignStmtEntity("7"), new VariableEntity("X")),
      new UsesRelationship(new AssignStmtEntity("8"), new VariableEntity("Z")),
      new UsesRelationship(new AssignStmtEntity("9"), new VariableEntity("B")),
      new CallsRelationship(new CallStmtEntity("10"), new ProcedureEntity("main")),
  };

  PKB pkb;
  pkb.populate(relationships);

  auto result_single_inverse_false = pkb.getByRelationship(RsType::kUses, new PrintStmtEntity("5"), false);
  auto result_single_inverse_true = pkb.getByRelationship(RsType::kUses, new VariableEntity("B"), true);
  auto result_invalid_procedure = pkb.getByRelationship(RsType::kUses, new ProcedureEntity("Megatron"), false);
  auto result_invalid_variable = pkb.getByRelationship(RsType::kUses, new VariableEntity("east"), true);
  auto result_multiple_inverse_false = pkb.getByRelationship(RsType::kUses, new AssignStmtEntity("1"), false);
  auto result_multiple_inverse_true = pkb.getByRelationship(RsType::kUses, new VariableEntity("X"), true);
  auto result_inference_from_while = pkb.getByRelationship(RsType::kUses, new WhileStmtEntity("2"), false);
  auto result_inference_from_if = pkb.getByRelationship(RsType::kUses, new IfStmtEntity("6"), false);
  auto result_inference_from_procedure = pkb.getByRelationship(RsType::kUses, new ProcedureEntity("main"), false);
  auto result_inference_from_call_stmt = pkb.getByRelationship(RsType::kUses, new CallStmtEntity("10"), false);
  auto result_inference_from_variable = pkb.getByRelationship(RsType::kUses, new VariableEntity("X"), true);

  EntityPointerUnorderedSet expected_result_single_inverse_false = {new VariableEntity("A")};
  EntityPointerUnorderedSet expected_result_single_inverse_true = {new AssignStmtEntity("9")};
  EntityPointerUnorderedSet expected_result_multiple_inverse_false = {new VariableEntity("X"), new VariableEntity("A")};
  EntityPointerUnorderedSet expected_result_multiple_inverse_true = {new AssignStmtEntity("1"),
                                                                     new AssignStmtEntity("7")};
  EntityPointerUnorderedSet expected_result_inference_from_while = {new VariableEntity("Y")};
  EntityPointerUnorderedSet expected_result_inference_from_if = {new VariableEntity("X"),
                                                                 new VariableEntity("Z")};
  EntityPointerUnorderedSet expected_result_all_variable_in_procedure = {new VariableEntity("A"),
                                                                         new VariableEntity("B"),
                                                                         new VariableEntity("X"),
                                                                         new VariableEntity("Y"),
                                                                         new VariableEntity("Z")};
  EntityPointerUnorderedSet expected_result_inference_from_variable = {new AssignStmtEntity("1"),
                                                                       new AssignStmtEntity("7")};
  EntityPointerUnorderedSet expected_result_empty = {};

  ASSERT_TRUE(PKBTestHelper::set_compare(result_single_inverse_false, expected_result_single_inverse_false));
  ASSERT_TRUE(PKBTestHelper::set_compare(result_single_inverse_true, expected_result_single_inverse_true));
  ASSERT_TRUE(PKBTestHelper::set_compare(result_invalid_procedure, expected_result_empty));
  ASSERT_TRUE(PKBTestHelper::set_compare(result_invalid_variable, expected_result_empty));
  ASSERT_TRUE(PKBTestHelper::set_compare(result_multiple_inverse_false, expected_result_multiple_inverse_false));
  ASSERT_TRUE(PKBTestHelper::set_compare(result_multiple_inverse_true, expected_result_multiple_inverse_true));
}

TEST(QueryTest, CallsRelationship) {
  std::vector<Relationship *> relationships = {
    new CallsRelationship(new ProcedureEntity("main"), new ProcedureEntity("one")),
    new CallsRelationship(new ProcedureEntity("one"), new ProcedureEntity("two")),
    new CallsRelationship(new ProcedureEntity("two"), new ProcedureEntity("three")),
    new CallsRelationship(new ProcedureEntity("three"), new ProcedureEntity("four")),
    new CallsRelationship(new ProcedureEntity("Autobots"), new ProcedureEntity("Megatron"))
  };

  PKB pkb;
  pkb.populate(relationships);

  auto result_single_inverse_false = pkb.getByRelationship(RsType::kCalls, new ProcedureEntity("main"), false);
  auto result_single_inverse_true = pkb.getByRelationship(RsType::kCalls, new ProcedureEntity("Megatron"), true);
  auto result_invalid_procedure = pkb.getByRelationship(RsType::kCalls, new ProcedureEntity("Bumblebee"), false);
  auto result_traverse_inverse_false = pkb.getByRelationship(RsType::kCallsAll, new ProcedureEntity("main"), false);
  auto result_traverse_inverse_true = pkb.getByRelationship(RsType::kCallsAll, new ProcedureEntity("four"), true);
  auto result_no_preceding_procedure = pkb.getByRelationship(RsType::kCalls, new ProcedureEntity("main"), true);
  auto result_no_subsequent_procedure = pkb.getByRelationship(RsType::kCalls, new ProcedureEntity("four"), false);

  EntityPointerUnorderedSet expected_result_single_inverse_false = {new ProcedureEntity("one")};
  EntityPointerUnorderedSet expected_result_single_inverse_true = {new ProcedureEntity("Autobots")};
  EntityPointerUnorderedSet expected_result_traverse_inverse_false = {new ProcedureEntity("one"), new ProcedureEntity("two"),
                                                                      new ProcedureEntity("three"), new ProcedureEntity("four")};
  EntityPointerUnorderedSet expected_result_traverse_inverse_true = {new ProcedureEntity("three"), new ProcedureEntity("two"),
                                                                     new ProcedureEntity("one"), new ProcedureEntity("main")};
  EntityPointerUnorderedSet expected_result_empty = {};

  ASSERT_TRUE(PKBTestHelper::set_compare(result_single_inverse_false, expected_result_single_inverse_false));
  ASSERT_TRUE(PKBTestHelper::set_compare(result_single_inverse_true, expected_result_single_inverse_true));
  ASSERT_TRUE(PKBTestHelper::set_compare(result_invalid_procedure, expected_result_empty));
  ASSERT_TRUE(PKBTestHelper::set_compare(result_traverse_inverse_false, expected_result_traverse_inverse_false));
  ASSERT_TRUE(PKBTestHelper::set_compare(result_traverse_inverse_true, expected_result_traverse_inverse_true));
  ASSERT_TRUE(PKBTestHelper::set_compare(result_no_preceding_procedure, expected_result_empty));
  ASSERT_TRUE(PKBTestHelper::set_compare(result_no_subsequent_procedure, expected_result_empty));
}

TEST(QueryTest, NextRelationship) {
  std::vector<Relationship *> relationship = {
    new NextRelationship(new AssignStmtEntity("1"), new AssignStmtEntity("2")),
    new NextRelationship(new AssignStmtEntity("2"), new AssignStmtEntity("3")),
    new NextRelationship(new AssignStmtEntity("3"), new AssignStmtEntity("4")),
    new NextRelationship(new AssignStmtEntity("4"), new AssignStmtEntity("5")),
    new NextRelationship(new AssignStmtEntity("8"), new AssignStmtEntity("10")),
  };

  PKB pkb;
  pkb.populate(relationship);

  auto result_single_inverse_false = pkb.getByRelationship(RsType::kNext, new AssignStmtEntity("1"), false);
  auto result_single_inverse_true = pkb.getByRelationship(RsType::kNext, new AssignStmtEntity("4"), true);
  auto result_invalid_assignment_statement = pkb.getByRelationship(RsType::kNext, new AssignStmtEntity("7"), false);
  auto result_traverse_inverse_false = pkb.getByRelationship(RsType::kNextAll, new AssignStmtEntity("1"), false);
  auto result_traverse_inverse_true = pkb.getByRelationship(RsType::kNextAll, new AssignStmtEntity("5"), true);
  auto result_no_preceding_statement = pkb.getByRelationship(RsType::kNext, new AssignStmtEntity("1"), true);
  auto result_no_subsequent_statement = pkb.getByRelationship(RsType::kNext, new AssignStmtEntity("10"), false);

  EntityPointerUnorderedSet expected_result_single_inverse_false = {new AssignStmtEntity("2")};
  EntityPointerUnorderedSet expected_result_single_inverse_true = {new AssignStmtEntity("3")};
  EntityPointerUnorderedSet expected_result_traverse_inverse_false = {new AssignStmtEntity("2"), new AssignStmtEntity("3"),
                                                                      new AssignStmtEntity("4"), new AssignStmtEntity("5")};
  EntityPointerUnorderedSet expected_result_traverse_inverse_true = {new AssignStmtEntity("4"), new AssignStmtEntity("3"),
                                                                     new AssignStmtEntity("2"), new AssignStmtEntity("1")};
  EntityPointerUnorderedSet expected_result_empty = {};

  ASSERT_TRUE(PKBTestHelper::set_compare(result_single_inverse_false, expected_result_single_inverse_false));
  ASSERT_TRUE(PKBTestHelper::set_compare(result_single_inverse_true, expected_result_single_inverse_true));
  ASSERT_TRUE(PKBTestHelper::set_compare(result_invalid_assignment_statement, expected_result_empty));
  ASSERT_TRUE(PKBTestHelper::set_compare(result_traverse_inverse_false, expected_result_traverse_inverse_false));
  ASSERT_TRUE(PKBTestHelper::set_compare(result_traverse_inverse_true, expected_result_traverse_inverse_true));
  ASSERT_TRUE(PKBTestHelper::set_compare(result_no_preceding_statement, expected_result_empty));
  ASSERT_TRUE(PKBTestHelper::set_compare(result_no_subsequent_statement, expected_result_empty));
}
