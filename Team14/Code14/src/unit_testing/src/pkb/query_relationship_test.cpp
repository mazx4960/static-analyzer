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
  auto result_traverse_inverse_false = pkb.getByRelationship(RsType::kFollowsT, new AssignStmtEntity("1"), false);
  auto result_traverse_inverse_true = pkb.getByRelationship(RsType::kFollowsT, new AssignStmtEntity("4"), true);
  auto result_no_preceding_statement = pkb.getByRelationship(RsType::kFollows, new AssignStmtEntity("1"), true);
  auto result_no_subsequent_statement = pkb.getByRelationship(RsType::kFollows, new AssignStmtEntity("10"), false);
  auto result_invalid_statement_inverse_false =
      pkb.getByRelationship(RsType::kFollows, new AssignStmtEntity("99"), false);
  auto result_invalid_statement_inverse_true =
      pkb.getByRelationship(RsType::kFollows, new AssignStmtEntity("99"), true);

  EntityPointerUnorderedSet expected_result_inverse_false = {new AssignStmtEntity("10")};
  EntityPointerUnorderedSet expected_result_inverse_true = {new AssignStmtEntity("8")};
  EntityPointerUnorderedSet expected_result_traverse_inverse_false = {
      new AssignStmtEntity("2"), new AssignStmtEntity("3"), new AssignStmtEntity("4")};
  EntityPointerUnorderedSet expected_result_traverse_inverse_true = {
      new AssignStmtEntity("1"), new AssignStmtEntity("2"), new AssignStmtEntity("3")};
  EntityPointerUnorderedSet expected_result_empty = {};

  ASSERT_TRUE(PKBTestHelper::set_compare(result_inverse_false, expected_result_inverse_false));
  ASSERT_TRUE(PKBTestHelper::set_compare(result_inverse_true, expected_result_inverse_true));
  ASSERT_TRUE(PKBTestHelper::set_compare(result_traverse_inverse_false, expected_result_traverse_inverse_false));
  ASSERT_TRUE(PKBTestHelper::set_compare(result_traverse_inverse_true, expected_result_traverse_inverse_true));
  ASSERT_TRUE(PKBTestHelper::set_compare(result_no_preceding_statement, expected_result_empty));
  ASSERT_TRUE(PKBTestHelper::set_compare(result_no_subsequent_statement, expected_result_empty));
  ASSERT_TRUE(PKBTestHelper::set_compare(result_invalid_statement_inverse_false, expected_result_empty));
  ASSERT_TRUE(PKBTestHelper::set_compare(result_invalid_statement_inverse_true, expected_result_empty));
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
  auto result_invalid_statement_inverse_false =
      pkb.getByRelationship(RsType::kParent, new AssignStmtEntity("99"), false);
  auto result_invalid_statement_inverse_true = pkb.getByRelationship(RsType::kParent, new AssignStmtEntity("99"), true);

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
  ASSERT_TRUE(PKBTestHelper::set_compare(result_invalid_statement_inverse_false, expected_result_empty));
  ASSERT_TRUE(PKBTestHelper::set_compare(result_invalid_statement_inverse_true, expected_result_empty));
}

TEST(QueryTest, ModifiesRelationship) {
  std::vector<Relationship *> relationships = {
      new ParentRelationship(new ProcedureEntity("main"), new AssignStmtEntity("1")),
      new ParentRelationship(new ProcedureEntity("main"), new WhileStmtEntity("2")),
      new ParentRelationship(new WhileStmtEntity("2"), new AssignStmtEntity("3")),
      new ParentRelationship(new ProcedureEntity("main"), new CallStmtEntity("4", std::string())),
      new ParentRelationship(new ProcedureEntity("main"), new ReadStmtEntity("5")),
      new ParentRelationship(new ProcedureEntity("main"), new IfStmtEntity("6")),
      new ParentRelationship(new IfStmtEntity("6"), new AssignStmtEntity("7")),
      new ParentRelationship(new IfStmtEntity("6"), new AssignStmtEntity("8")),
      new ParentRelationship(new ProcedureEntity("main"), new AssignStmtEntity("9")),
      new ParentRelationship(new ProcedureEntity("second_procedure"), new CallStmtEntity("10", std::string())),
      new ParentRelationship(new ProcedureEntity("second_procedure"), new AssignStmtEntity("11")),
      new ModifiesRelationship(new AssignStmtEntity("1"), new VariableEntity("X")),
      new ModifiesRelationship(new AssignStmtEntity("3"), new VariableEntity("Y")),
      new ModifiesRelationship(new ReadStmtEntity("5"), new VariableEntity("A")),
      new ModifiesRelationship(new AssignStmtEntity("7"), new VariableEntity("X")),
      new ModifiesRelationship(new AssignStmtEntity("8"), new VariableEntity("Z")),
      new ModifiesRelationship(new AssignStmtEntity("9"), new VariableEntity("B")),
      new ModifiesRelationship(new AssignStmtEntity("11"), new VariableEntity("cenX")),
      new CallsRelationship(new CallStmtEntity("10", std::string()), new ProcedureEntity("main")),
  };

  PKB pkb;
  pkb.populate(relationships);

  auto result_inverse_false = pkb.getByRelationship(RsType::kModifies, new ReadStmtEntity("5"), false);
  auto result_inverse_true = pkb.getByRelationship(RsType::kModifies, new VariableEntity("B"), true);
  auto result_invalid_procedure = pkb.getByRelationship(RsType::kModifies, new ProcedureEntity("Megatron"), false);
  auto result_invalid_variable = pkb.getByRelationship(RsType::kModifies, new VariableEntity("C"), false);
  auto result_invalid_read = pkb.getByRelationship(RsType::kModifies, new ReadStmtEntity("15"), false);
  auto result_invalid_if = pkb.getByRelationship(RsType::kModifies, new IfStmtEntity("16"), false);
  auto result_invalid_while = pkb.getByRelationship(RsType::kModifies, new WhileStmtEntity("17"), false);
  auto result_multiple_inverse_true = pkb.getByRelationship(RsType::kModifies, new VariableEntity("X"), true);
  auto result_inference_from_while = pkb.getByRelationship(RsType::kModifies, new WhileStmtEntity("2"), false);
  auto result_inference_from_if = pkb.getByRelationship(RsType::kModifies, new IfStmtEntity("6"), false);
  auto result_inference_from_procedure = pkb.getByRelationship(RsType::kModifies, new ProcedureEntity("main"), false);
  auto result_inference_from_call_stmt =
      pkb.getByRelationship(RsType::kModifies, new CallStmtEntity("10", std::string()), false);
  auto result_inference_from_variable = pkb.getByRelationship(RsType::kModifies, new VariableEntity("Y"), true);
  auto result_another_procedure =
      pkb.getByRelationship(RsType::kModifies, new ProcedureEntity("second_procedure"), false);

  EntityPointerUnorderedSet expected_result_inverse_false = {new VariableEntity("A")};
  EntityPointerUnorderedSet expected_result_inverse_true = {new AssignStmtEntity("9"), new ProcedureEntity("main"),
                                                            new ProcedureEntity("second_procedure")};
  EntityPointerUnorderedSet expected_result_multiple_inverse_true = {
      new AssignStmtEntity("1"), new AssignStmtEntity("7"), new ProcedureEntity("main"),
      new ProcedureEntity("second_procedure")};
  EntityPointerUnorderedSet expected_result_inference_from_while = {new VariableEntity("Y")};
  EntityPointerUnorderedSet expected_result_inference_from_if = {new VariableEntity("X"), new VariableEntity("Z")};
  EntityPointerUnorderedSet expected_result_all_variable_in_procedure = {
      new VariableEntity("A"), new VariableEntity("B"), new VariableEntity("X"), new VariableEntity("Y"),
      new VariableEntity("Z")};
  EntityPointerUnorderedSet expected_result_inference_from_variable = {
      new AssignStmtEntity("3"), new ProcedureEntity("main"), new ProcedureEntity("second_procedure")};
  EntityPointerUnorderedSet expected_result_another_procedure = {
      new VariableEntity("A"), new VariableEntity("B"),    new VariableEntity("X"),    new VariableEntity("Y"),
      new VariableEntity("Z"), new VariableEntity("cenX"), new ProcedureEntity("main")};
  EntityPointerUnorderedSet expected_result_empty = {};

  ASSERT_TRUE(PKBTestHelper::set_compare(result_inverse_false, expected_result_inverse_false));
  ASSERT_TRUE(PKBTestHelper::set_compare(result_inverse_true, expected_result_inverse_true));
  ASSERT_TRUE(PKBTestHelper::set_compare(result_invalid_procedure, expected_result_empty));
  ASSERT_TRUE(PKBTestHelper::set_compare(result_invalid_variable, expected_result_empty));
  ASSERT_TRUE(PKBTestHelper::set_compare(result_invalid_read, expected_result_empty));
  ASSERT_TRUE(PKBTestHelper::set_compare(result_invalid_if, expected_result_empty));
  ASSERT_TRUE(PKBTestHelper::set_compare(result_invalid_while, expected_result_empty));
  ASSERT_TRUE(PKBTestHelper::set_compare(result_multiple_inverse_true, expected_result_multiple_inverse_true));
  ASSERT_TRUE(PKBTestHelper::set_compare(result_inference_from_while, expected_result_inference_from_while));
  ASSERT_TRUE(PKBTestHelper::set_compare(result_inference_from_if, expected_result_inference_from_if));
  ASSERT_TRUE(PKBTestHelper::set_compare(result_inference_from_procedure, expected_result_all_variable_in_procedure));
  ASSERT_TRUE(PKBTestHelper::set_compare(result_inference_from_call_stmt, expected_result_all_variable_in_procedure));
  ASSERT_TRUE(PKBTestHelper::set_compare(result_inference_from_variable, expected_result_inference_from_variable));
  ASSERT_TRUE(PKBTestHelper::set_compare(result_another_procedure, expected_result_another_procedure));
}

TEST(QueryTest, UsesRelationship) {
  std::vector<Relationship *> relationships = {
      new ParentRelationship(new ProcedureEntity("main"), new AssignStmtEntity("1")),
      new ParentRelationship(new ProcedureEntity("main"), new WhileStmtEntity("2")),
      new ParentRelationship(new WhileStmtEntity("2"), new AssignStmtEntity("3")),
      new ParentRelationship(new ProcedureEntity("main"), new CallStmtEntity("4", std::string())),
      new ParentRelationship(new ProcedureEntity("main"), new PrintStmtEntity("5")),
      new ParentRelationship(new ProcedureEntity("main"), new IfStmtEntity("6")),
      new ParentRelationship(new IfStmtEntity("6"), new AssignStmtEntity("7")),
      new ParentRelationship(new IfStmtEntity("6"), new AssignStmtEntity("8")),
      new ParentRelationship(new ProcedureEntity("main"), new AssignStmtEntity("9")),
      new ParentRelationship(new ProcedureEntity("second_procedure"), new CallStmtEntity("10", std::string())),
      new ParentRelationship(new ProcedureEntity("second_procedure"), new PrintStmtEntity("11")),
      new UsesRelationship(new AssignStmtEntity("1"), new VariableEntity("X")),
      new UsesRelationship(new AssignStmtEntity("1"), new VariableEntity("A")),
      new UsesRelationship(new AssignStmtEntity("3"), new VariableEntity("Y")),
      new UsesRelationship(new PrintStmtEntity("5"), new VariableEntity("A")),
      new UsesRelationship(new AssignStmtEntity("7"), new VariableEntity("X")),
      new UsesRelationship(new AssignStmtEntity("8"), new VariableEntity("Z")),
      new UsesRelationship(new AssignStmtEntity("9"), new VariableEntity("B")),
      new UsesRelationship(new PrintStmtEntity("11"), new VariableEntity("cenX")),
      new CallsRelationship(new CallStmtEntity("10", std::string()), new ProcedureEntity("main")),
  };

  PKB pkb;
  pkb.populate(relationships);

  auto result_inverse_false = pkb.getByRelationship(RsType::kUses, new PrintStmtEntity("5"), false);
  auto result_inverse_true = pkb.getByRelationship(RsType::kUses, new VariableEntity("B"), true);
  auto result_invalid_procedure = pkb.getByRelationship(RsType::kUses, new ProcedureEntity("Megatron"), false);
  auto result_invalid_variable = pkb.getByRelationship(RsType::kUses, new VariableEntity("east"), true);
  auto result_invalid_print = pkb.getByRelationship(RsType::kUses, new PrintStmtEntity("15"), false);
  auto result_invalid_if = pkb.getByRelationship(RsType::kUses, new IfStmtEntity("16"), false);
  auto result_invalid_while = pkb.getByRelationship(RsType::kUses, new WhileStmtEntity("17"), false);
  auto result_multiple_inverse_false = pkb.getByRelationship(RsType::kUses, new AssignStmtEntity("1"), false);
  auto result_multiple_inverse_true = pkb.getByRelationship(RsType::kUses, new VariableEntity("X"), true);
  auto result_inference_from_while = pkb.getByRelationship(RsType::kUses, new WhileStmtEntity("2"), false);
  auto result_inference_from_if = pkb.getByRelationship(RsType::kUses, new IfStmtEntity("6"), false);
  auto result_inference_from_procedure = pkb.getByRelationship(RsType::kUses, new ProcedureEntity("main"), false);
  auto result_inference_from_call_stmt =
      pkb.getByRelationship(RsType::kUses, new CallStmtEntity("10", std::string()), false);
  auto result_inference_from_variable = pkb.getByRelationship(RsType::kUses, new VariableEntity("X"), true);
  auto result_another_procedure = pkb.getByRelationship(RsType::kUses, new ProcedureEntity("second_procedure"), false);
  auto a = pkb.getByRelationship(RsType::kUses, new ProcedureEntity("main"), true);

  EntityPointerUnorderedSet expected_result_inverse_false = {new VariableEntity("A")};
  EntityPointerUnorderedSet expected_result_inverse_true = {new AssignStmtEntity("9"), new ProcedureEntity("main"),
                                                            new ProcedureEntity("second_procedure")};
  EntityPointerUnorderedSet expected_result_multiple_inverse_false = {new VariableEntity("X"), new VariableEntity("A")};
  EntityPointerUnorderedSet expected_result_multiple_inverse_true = {
      new AssignStmtEntity("1"), new AssignStmtEntity("7"), new ProcedureEntity("main"),
      new ProcedureEntity("second_procedure")};
  EntityPointerUnorderedSet expected_result_inference_from_while = {new VariableEntity("Y")};
  EntityPointerUnorderedSet expected_result_inference_from_if = {new VariableEntity("X"), new VariableEntity("Z")};
  EntityPointerUnorderedSet expected_result_all_variable_in_procedure = {
      new VariableEntity("A"), new VariableEntity("B"), new VariableEntity("X"), new VariableEntity("Y"),
      new VariableEntity("Z")};
  EntityPointerUnorderedSet expected_result_inference_from_variable = {
      new AssignStmtEntity("1"), new AssignStmtEntity("7"), new ProcedureEntity("main"),
      new ProcedureEntity("second_procedure")};
  EntityPointerUnorderedSet expected_result_another_procedure = {
      new VariableEntity("A"), new VariableEntity("B"),    new VariableEntity("X"),    new VariableEntity("Y"),
      new VariableEntity("Z"), new VariableEntity("cenX"), new ProcedureEntity("main")};
  EntityPointerUnorderedSet expected_result_empty = {};

  ASSERT_TRUE(PKBTestHelper::set_compare(result_inverse_false, expected_result_inverse_false));
  ASSERT_TRUE(PKBTestHelper::set_compare(result_inverse_true, expected_result_inverse_true));
  ASSERT_TRUE(PKBTestHelper::set_compare(result_invalid_procedure, expected_result_empty));
  ASSERT_TRUE(PKBTestHelper::set_compare(result_invalid_variable, expected_result_empty));
  ASSERT_TRUE(PKBTestHelper::set_compare(result_invalid_print, expected_result_empty));
  ASSERT_TRUE(PKBTestHelper::set_compare(result_invalid_if, expected_result_empty));
  ASSERT_TRUE(PKBTestHelper::set_compare(result_invalid_while, expected_result_empty));
  ASSERT_TRUE(PKBTestHelper::set_compare(result_multiple_inverse_false, expected_result_multiple_inverse_false));
  ASSERT_TRUE(PKBTestHelper::set_compare(result_multiple_inverse_true, expected_result_multiple_inverse_true));
  ASSERT_TRUE(PKBTestHelper::set_compare(result_inference_from_while, expected_result_inference_from_while));
  ASSERT_TRUE(PKBTestHelper::set_compare(result_inference_from_if, expected_result_inference_from_if));
  ASSERT_TRUE(PKBTestHelper::set_compare(result_inference_from_procedure, expected_result_all_variable_in_procedure));
  ASSERT_TRUE(PKBTestHelper::set_compare(result_inference_from_call_stmt, expected_result_all_variable_in_procedure));
  ASSERT_TRUE(PKBTestHelper::set_compare(result_inference_from_variable, expected_result_inference_from_variable));
  ASSERT_TRUE(PKBTestHelper::set_compare(result_another_procedure, expected_result_another_procedure));
}

TEST(QueryTest, CallsRelationship) {
  std::vector<Relationship *> relationships = {
      new CallsRelationship(new CallStmtEntity("1", std::string()), new ProcedureEntity("Autobots")),
      new CallsRelationship(new CallStmtEntity("2", std::string()), new ProcedureEntity("B")),
      new CallsRelationship(new CallStmtEntity("3", std::string()), new ProcedureEntity("C")),
      new CallsRelationship(new CallStmtEntity("4", std::string()), new ProcedureEntity("D")),
      new CallsRelationship(new CallStmtEntity("8", std::string()), new ProcedureEntity("E")),
      new ParentRelationship(new ProcedureEntity("main"), new CallStmtEntity("1", std::string())),
      new ParentRelationship(new ProcedureEntity("main"), new CallStmtEntity("2", std::string())),
      new ParentRelationship(new ProcedureEntity("A"), new CallStmtEntity("3", std::string())),
      new ParentRelationship(new ProcedureEntity("B"), new CallStmtEntity("4", std::string())),
      new ParentRelationship(new ProcedureEntity("D"), new CallStmtEntity("8", std::string())),
  };

  PKB pkb;
  pkb.populate(relationships);

  auto result_single_inverse_false = pkb.getByRelationship(RsType::kCalls, new ProcedureEntity("D"), false);
  auto result_single_inverse_true = pkb.getByRelationship(RsType::kCalls, new ProcedureEntity("E"), true);
  auto result_invalid_procedure = pkb.getByRelationship(RsType::kCalls, new ProcedureEntity("Bumblebee"), false);
  auto result_traverse_inverse_false = pkb.getByRelationship(RsType::kCallsT, new ProcedureEntity("main"), false);
  auto result_traverse_inverse_true = pkb.getByRelationship(RsType::kCallsT, new ProcedureEntity("E"), true);
  auto result_no_preceding_procedure = pkb.getByRelationship(RsType::kCalls, new ProcedureEntity("main"), true);
  auto result_no_subsequent_procedure = pkb.getByRelationship(RsType::kCalls, new ProcedureEntity("E"), false);

  EntityPointerUnorderedSet expected_result_single_inverse_false = {new ProcedureEntity("E")};
  EntityPointerUnorderedSet expected_result_single_inverse_true = {new ProcedureEntity("D")};
  EntityPointerUnorderedSet expected_result_traverse_inverse_false = {
      new ProcedureEntity("Autobots"), new ProcedureEntity("B"), new ProcedureEntity("D"), new ProcedureEntity("E")};
  EntityPointerUnorderedSet expected_result_traverse_inverse_true = {new ProcedureEntity("D"), new ProcedureEntity("B"),
                                                                     new ProcedureEntity("main")};
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
  auto result_traverse_inverse_false = pkb.getByRelationship(RsType::kNextT, new AssignStmtEntity("1"), false);
  auto result_traverse_inverse_true = pkb.getByRelationship(RsType::kNextT, new AssignStmtEntity("5"), true);
  auto result_no_preceding_statement = pkb.getByRelationship(RsType::kNext, new AssignStmtEntity("1"), true);
  auto result_no_subsequent_statement = pkb.getByRelationship(RsType::kNext, new AssignStmtEntity("10"), false);

  EntityPointerUnorderedSet expected_result_single_inverse_false = {new AssignStmtEntity("2")};
  EntityPointerUnorderedSet expected_result_single_inverse_true = {new AssignStmtEntity("3")};
  EntityPointerUnorderedSet expected_result_traverse_inverse_false = {
      new AssignStmtEntity("2"), new AssignStmtEntity("3"), new AssignStmtEntity("4"), new AssignStmtEntity("5")};
  EntityPointerUnorderedSet expected_result_traverse_inverse_true = {
      new AssignStmtEntity("4"), new AssignStmtEntity("3"), new AssignStmtEntity("2"), new AssignStmtEntity("1")};
  EntityPointerUnorderedSet expected_result_empty = {};

  ASSERT_TRUE(PKBTestHelper::set_compare(result_single_inverse_false, expected_result_single_inverse_false));
  ASSERT_TRUE(PKBTestHelper::set_compare(result_single_inverse_true, expected_result_single_inverse_true));
  ASSERT_TRUE(PKBTestHelper::set_compare(result_invalid_assignment_statement, expected_result_empty));
  ASSERT_TRUE(PKBTestHelper::set_compare(result_traverse_inverse_false, expected_result_traverse_inverse_false));
  ASSERT_TRUE(PKBTestHelper::set_compare(result_traverse_inverse_true, expected_result_traverse_inverse_true));
  ASSERT_TRUE(PKBTestHelper::set_compare(result_no_preceding_statement, expected_result_empty));
  ASSERT_TRUE(PKBTestHelper::set_compare(result_no_subsequent_statement, expected_result_empty));
}

TEST(QueryTest, AffectsRelationship) {
  std::vector<Relationship *> relationships = {
      new NextRelationship(new AssignStmtEntity("1"), new AssignStmtEntity("2")),
      new NextRelationship(new AssignStmtEntity("2"), new AssignStmtEntity("3")),
      new NextRelationship(new AssignStmtEntity("3"), new AssignStmtEntity("4")),
      new NextRelationship(new AssignStmtEntity("4"), new AssignStmtEntity("5")),
      new NextRelationship(new AssignStmtEntity("5"), new AssignStmtEntity("6")),
      new NextRelationship(new AssignStmtEntity("6"), new WhileStmtEntity("7")),
      new NextRelationship(new WhileStmtEntity("7"), new IfStmtEntity("8")),
      new NextRelationship(new IfStmtEntity("8"), new AssignStmtEntity("9")),
      new NextRelationship(new AssignStmtEntity("9"), new AssignStmtEntity("10")),
      new NextRelationship(new AssignStmtEntity("10"), new CallStmtEntity("11", std::string())),
      new NextRelationship(new IfStmtEntity("8"), new AssignStmtEntity("12")),
      new NextRelationship(new AssignStmtEntity("12"), new AssignStmtEntity("13")),
      new NextRelationship(new CallStmtEntity("11", std::string()), new AssignStmtEntity("14")),
      new NextRelationship(new AssignStmtEntity("13"), new AssignStmtEntity("14")),
      new NextRelationship(new AssignStmtEntity("14"), new ReadStmtEntity("15")),
      new NextRelationship(new ReadStmtEntity("15"), new PrintStmtEntity("16")),
      new NextRelationship(new PrintStmtEntity("16"), new WhileStmtEntity("7")),
      new NextRelationship(new PrintStmtEntity("16"), new AssignStmtEntity("17")),
      new NextRelationship(new AssignStmtEntity("17"), new IfStmtEntity("18")),
      new NextRelationship(new IfStmtEntity("18"), new CallStmtEntity("19", std::string())),
      new NextRelationship(new IfStmtEntity("18"), new AssignStmtEntity("20")),
      new NextRelationship(new CallStmtEntity("19", std::string()), new AssignStmtEntity("21")),
      new NextRelationship(new AssignStmtEntity("20"), new AssignStmtEntity("21")),
      new NextRelationship(new AssignStmtEntity("21"), new AssignStmtEntity("22")),
      new NextRelationship(new AssignStmtEntity("23"), new AssignStmtEntity("24")),
      new NextRelationship(new AssignStmtEntity("24"), new AssignStmtEntity("25")),

      new ModifiesRelationship(new AssignStmtEntity("1"), new VariableEntity("x")),
      new ModifiesRelationship(new AssignStmtEntity("2"), new VariableEntity("y")),
      new ModifiesRelationship(new AssignStmtEntity("3"), new VariableEntity("a")),
      new ModifiesRelationship(new AssignStmtEntity("4"), new VariableEntity("z")),
      new ModifiesRelationship(new AssignStmtEntity("5"), new VariableEntity("b")),
      new ModifiesRelationship(new AssignStmtEntity("6"), new VariableEntity("n")),
      new ModifiesRelationship(new AssignStmtEntity("9"), new VariableEntity("m")),
      new ModifiesRelationship(new AssignStmtEntity("10"), new VariableEntity("n")),
      new ModifiesRelationship(new AssignStmtEntity("12"), new VariableEntity("y")),
      new ModifiesRelationship(new AssignStmtEntity("13"), new VariableEntity("east")),
      new ModifiesRelationship(new AssignStmtEntity("14"), new VariableEntity("x")),
      new ModifiesRelationship(new ReadStmtEntity("15"), new VariableEntity("y")),
      new ModifiesRelationship(new AssignStmtEntity("17"), new VariableEntity("c")),
      new ModifiesRelationship(new AssignStmtEntity("20"), new VariableEntity("c")),
      new ModifiesRelationship(new AssignStmtEntity("21"), new VariableEntity("z")),
      new ModifiesRelationship(new AssignStmtEntity("22"), new VariableEntity("cenX")),
      new ModifiesRelationship(new AssignStmtEntity("23"), new VariableEntity("x")),
      new ModifiesRelationship(new AssignStmtEntity("24"), new VariableEntity("y")),
      new ModifiesRelationship(new AssignStmtEntity("25"), new VariableEntity("z")),

      new UsesRelationship(new AssignStmtEntity("2"), new VariableEntity("x")),
      new UsesRelationship(new AssignStmtEntity("2"), new VariableEntity("a")),
      new UsesRelationship(new AssignStmtEntity("5"), new VariableEntity("b")),
      new UsesRelationship(new WhileStmtEntity("7"), new VariableEntity("x")),
      new UsesRelationship(new IfStmtEntity("8"), new VariableEntity("z")),
      new UsesRelationship(new AssignStmtEntity("9"), new VariableEntity("x")),
      new UsesRelationship(new AssignStmtEntity("9"), new VariableEntity("z")),
      new UsesRelationship(new AssignStmtEntity("10"), new VariableEntity("n")),
      new UsesRelationship(new AssignStmtEntity("13"), new VariableEntity("y")),
      new UsesRelationship(new AssignStmtEntity("13"), new VariableEntity("a")),
      new UsesRelationship(new AssignStmtEntity("14"), new VariableEntity("y")),
      new UsesRelationship(new AssignStmtEntity("14"), new VariableEntity("z")),
      new UsesRelationship(new AssignStmtEntity("14"), new VariableEntity("a")),
      new UsesRelationship(new PrintStmtEntity("16"), new VariableEntity("x")),
      new UsesRelationship(new IfStmtEntity("18"), new VariableEntity("m")),
      new UsesRelationship(new AssignStmtEntity("20"), new VariableEntity("x")),
      new UsesRelationship(new AssignStmtEntity("21"), new VariableEntity("c")),
      new UsesRelationship(new AssignStmtEntity("22"), new VariableEntity("z")),
      new UsesRelationship(new AssignStmtEntity("22"), new VariableEntity("a")),
      new UsesRelationship(new AssignStmtEntity("24"), new VariableEntity("x")),
      new UsesRelationship(new AssignStmtEntity("25"), new VariableEntity("y")),

      new ParentRelationship(new ProcedureEntity("main"), new AssignStmtEntity("1")),
      new ParentRelationship(new ProcedureEntity("main"), new AssignStmtEntity("2")),
      new ParentRelationship(new ProcedureEntity("main"), new AssignStmtEntity("3")),
      new ParentRelationship(new ProcedureEntity("main"), new AssignStmtEntity("4")),
      new ParentRelationship(new ProcedureEntity("main"), new AssignStmtEntity("5")),
      new ParentRelationship(new ProcedureEntity("main"), new AssignStmtEntity("6")),
      new ParentRelationship(new ProcedureEntity("main"), new WhileStmtEntity("7")),
      new ParentRelationship(new WhileStmtEntity("7"), new IfStmtEntity("8")),
      new ParentRelationship(new WhileStmtEntity("7"), new AssignStmtEntity("14")),
      new ParentRelationship(new WhileStmtEntity("7"), new ReadStmtEntity("15")),
      new ParentRelationship(new WhileStmtEntity("7"), new PrintStmtEntity("16")),
      new ParentRelationship(new IfStmtEntity("8"), new AssignStmtEntity("9")),
      new ParentRelationship(new IfStmtEntity("8"), new AssignStmtEntity("10")),
      new ParentRelationship(new IfStmtEntity("8"), new CallStmtEntity("11", std::string())),
      new ParentRelationship(new IfStmtEntity("8"), new AssignStmtEntity("12")),
      new ParentRelationship(new IfStmtEntity("8"), new CallStmtEntity("13", std::string())),
      new ParentRelationship(new ProcedureEntity("main"), new AssignStmtEntity("17")),
      new ParentRelationship(new ProcedureEntity("main"), new IfStmtEntity("18")),
      new ParentRelationship(new IfStmtEntity("18"), new CallStmtEntity("19", std::string())),
      new ParentRelationship(new IfStmtEntity("18"), new AssignStmtEntity("20")),
      new ParentRelationship(new ProcedureEntity("main"), new AssignStmtEntity("21")),
      new ParentRelationship(new ProcedureEntity("main"), new AssignStmtEntity("22")),
      new ParentRelationship(new ProcedureEntity("second"), new AssignStmtEntity("23")),
      new ParentRelationship(new ProcedureEntity("second"), new AssignStmtEntity("24")),
      new ParentRelationship(new ProcedureEntity("second"), new AssignStmtEntity("25")),

      new CallsRelationship(new CallStmtEntity("11", std::string()), new ProcedureEntity("second")),
      new CallsRelationship(new CallStmtEntity("19", std::string()), new ProcedureEntity("second")),
  };

  PKB pkb;
  pkb.populate(relationships);

  auto result_get_uses_simple = pkb.getByRelationship(RsType::kAffects, new AssignStmtEntity("20"), false);
  auto result_get_uses_block_by_call = pkb.getByRelationship(RsType::kAffects, new AssignStmtEntity("2"), false);
  auto result_get_uses_multiple = pkb.getByRelationship(RsType::kAffects, new AssignStmtEntity("1"), false);
  auto result_get_uses_same_stmt = pkb.getByRelationship(RsType::kAffects, new AssignStmtEntity("5"), false);
  auto result_get_uses_same_stmt_in_loop = pkb.getByRelationship(RsType::kAffects, new AssignStmtEntity("10"), false);
  auto result_get_uses_none = pkb.getByRelationship(RsType::kAffects, new AssignStmtEntity("16"), false);
  auto result_get_uses_split_on_if = pkb.getByRelationship(RsType::kAffects, new AssignStmtEntity("6"), false);
  auto result_get_uses_chain_simple = pkb.getByRelationship(RsType::kAffectsT, new AssignStmtEntity("23"), false);
  auto result_get_uses_chain_multiple = pkb.getByRelationship(RsType::kAffectsT, new AssignStmtEntity("12"), false);
  auto result_get_modify_simple = pkb.getByRelationship(RsType::kAffects, new AssignStmtEntity("2"), true);
  auto result_get_modify_multiple = pkb.getByRelationship(RsType::kAffects, new AssignStmtEntity("22"), true);
  auto result_get_modify_multiple_in_loop = pkb.getByRelationship(RsType::kAffects, new AssignStmtEntity("13"), true);
  auto result_get_modify_none = pkb.getByRelationship(RsType::kAffects, new AssignStmtEntity("4"), true);
  auto result_get_modify_chain_simple = pkb.getByRelationship(RsType::kAffectsT, new AssignStmtEntity("25"), true);

  EntityPointerUnorderedSet expected_result_get_uses_simple = {new AssignStmtEntity("21")};
  EntityPointerUnorderedSet expected_result_get_uses_multiple = {new AssignStmtEntity("2"), new AssignStmtEntity("9")};
  EntityPointerUnorderedSet expected_result_get_uses_same_stmt_in_loop = {new AssignStmtEntity("10")};
  EntityPointerUnorderedSet expected_result_get_uses_chain_simple = {new AssignStmtEntity("24"),
                                                                     new AssignStmtEntity("25")};
  EntityPointerUnorderedSet expected_result_get_uses_chain_multiple = {
      new AssignStmtEntity("9"),  new AssignStmtEntity("13"), new AssignStmtEntity("14"),
      new AssignStmtEntity("20"), new AssignStmtEntity("21"), new AssignStmtEntity("22")};
  EntityPointerUnorderedSet expected_result_get_uses_split_on_if = {new AssignStmtEntity("10")};
  EntityPointerUnorderedSet expected_result_get_modify_simple = {new AssignStmtEntity("1")};
  EntityPointerUnorderedSet expected_result_get_modify_multiple = {new AssignStmtEntity("21"),
                                                                   new AssignStmtEntity("3")};
  EntityPointerUnorderedSet expected_result_get_modify_multiple_in_loop = {new AssignStmtEntity("12"),
                                                                           new AssignStmtEntity("3")};
  EntityPointerUnorderedSet expected_result_get_modify_chain_simple = {new AssignStmtEntity("24"),
                                                                       new AssignStmtEntity("23")};

  EntityPointerUnorderedSet expected_result_empty = {};

  ASSERT_TRUE(PKBTestHelper::set_compare(result_get_uses_simple, expected_result_get_uses_simple));
  ASSERT_TRUE(PKBTestHelper::set_compare(result_get_uses_block_by_call, expected_result_empty));
  ASSERT_TRUE(PKBTestHelper::set_compare(result_get_uses_multiple, expected_result_get_uses_multiple));
  ASSERT_TRUE(PKBTestHelper::set_compare(result_get_uses_same_stmt, expected_result_empty));
  ASSERT_TRUE(
      PKBTestHelper::set_compare(result_get_uses_same_stmt_in_loop, expected_result_get_uses_same_stmt_in_loop));
  ASSERT_TRUE(PKBTestHelper::set_compare(result_get_uses_none, expected_result_empty));
  ASSERT_TRUE(PKBTestHelper::set_compare(result_get_uses_split_on_if, expected_result_get_uses_split_on_if));
  ASSERT_TRUE(PKBTestHelper::set_compare(result_get_uses_chain_simple, expected_result_get_uses_chain_simple));
  ASSERT_TRUE(PKBTestHelper::set_compare(result_get_uses_chain_multiple, expected_result_get_uses_chain_multiple));
  ASSERT_TRUE(PKBTestHelper::set_compare(result_get_modify_simple, expected_result_get_modify_simple));
  ASSERT_TRUE(PKBTestHelper::set_compare(result_get_modify_multiple, expected_result_get_modify_multiple));
  ASSERT_TRUE(
      PKBTestHelper::set_compare(result_get_modify_multiple_in_loop, expected_result_get_modify_multiple_in_loop));
  ASSERT_TRUE(PKBTestHelper::set_compare(result_get_modify_none, expected_result_empty));
  ASSERT_TRUE(PKBTestHelper::set_compare(result_get_modify_chain_simple, expected_result_get_modify_chain_simple));
}
