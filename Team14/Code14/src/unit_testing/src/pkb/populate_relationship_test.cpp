// Copyright 2022 CS3203 Team14. All rights reserved.

#include <gtest/gtest.h>

#include <vector>

#include "commons/entity.h"
#include "commons/relationship.h"
#include "pkb/pkb.h"
#include "pkb_test_helper.h"

using EntityPointerUnorderedSet = std::unordered_set<Entity *, EntityHashFunction, EntityPointerEquality>;

/*
* Same statement modify different variables 
*/
TEST(PopulateRelationshipTest, ModifiesSingleStatementTest) {
  std::vector<std::pair<std::string, std::string>> modifies = {{"1", "w"}, 
                                                               {"1", "x"}, 
                                                               {"1", "y"}, 
                                                               {"1", "z"}};
  std::vector<Relationship *> relationships;
  int length = modifies.size();
  relationships.reserve(length);
  EntityPointerUnorderedSet expected;
  EntityPointerUnorderedSet expected_inverse;

  for (int i = 0; i < length; i++) {
    relationships.push_back(
        new ModifiesRelationship(new AssignStmtEntity(modifies[i].first), 
                                 new VariableEntity(modifies[i].second)));
    expected.insert(new VariableEntity(modifies[i].second));
    expected_inverse.insert(new AssignStmtEntity(modifies[i].first));
  }

  auto *relationship_manager = new RelationshipManager();
  relationship_manager->Populate(relationships);
  auto *relationship_table = relationship_manager->GetTable(RsType::kModifies);
  bool compare = 
      PKBTestHelper::set_compare(expected, relationship_table->get(new AssignStmtEntity("1"), false));
  bool compare_inverse_w = 
      PKBTestHelper::set_compare(expected_inverse, relationship_table->get(new VariableEntity("w"), true));
  bool compare_inverse_x =
      PKBTestHelper::set_compare(expected_inverse, relationship_table->get(new VariableEntity("x"), true));
  bool compare_inverse_y =
      PKBTestHelper::set_compare(expected_inverse, relationship_table->get(new VariableEntity("y"), true));
  bool compare_inverse_z =
      PKBTestHelper::set_compare(expected_inverse, relationship_table->get(new VariableEntity("z"), true));


  int count = 0;
  int inverse_count = 0;
  int key_count = 0;
  int inverse_key_count = 0;
  EntityPointerUnorderedMap table = relationship_table->GetTable();
  EntityPointerUnorderedMap inverse_table = relationship_table->GetTable(true);

  for (auto &key : table) {
    count += key.second.size();
    key_count++;
  }

  for (auto &inverse_key : inverse_table) {
    inverse_count += inverse_key.second.size();
    inverse_key_count++;
  }

  ASSERT_EQ(count, 4);
  ASSERT_EQ(inverse_count, 4);
  ASSERT_EQ(key_count, 1);
  ASSERT_EQ(inverse_key_count, 4);
  ASSERT_EQ(compare, true);
  ASSERT_EQ(compare_inverse_w, true);
  ASSERT_EQ(compare_inverse_x, true);
  ASSERT_EQ(compare_inverse_y, true);
  ASSERT_EQ(compare_inverse_z, true);
}

/*
* Same procedure modify different variables 
*/
TEST(PopulateRelationshipTest, ModifiesSingleProcedureTest) {
  std::vector<std::pair<std::string, std::string>> modifies = {{"One", "w"}, 
                                                               {"One", "x"}, 
                                                               {"One", "y"}, 
                                                               {"One", "z"}};
  std::vector<Relationship *> relationships;
  int length = modifies.size();
  relationships.reserve(length);
  EntityPointerUnorderedSet expected;
  EntityPointerUnorderedSet expected_inverse;

  for (int i = 0; i < length; i++) {
    relationships.push_back(
        new ModifiesRelationship(new ProcedureEntity(modifies[i].first), 
                                 new VariableEntity(modifies[i].second)));
    expected.insert(new VariableEntity(modifies[i].second));
    expected_inverse.insert(new ProcedureEntity(modifies[i].first));
  }

  auto *relationship_manager = new RelationshipManager();
  relationship_manager->Populate(relationships);
  auto *relationship_table = relationship_manager->GetTable(RsType::kModifies);
  bool compare = PKBTestHelper::set_compare(expected, relationship_table->get(new ProcedureEntity("One"), false));
  bool compare_inverse_w =
      PKBTestHelper::set_compare(expected_inverse, relationship_table->get(new VariableEntity("w"), true));
  bool compare_inverse_x =
      PKBTestHelper::set_compare(expected_inverse, relationship_table->get(new VariableEntity("x"), true));
  bool compare_inverse_y =
      PKBTestHelper::set_compare(expected_inverse, relationship_table->get(new VariableEntity("y"), true));
  bool compare_inverse_z =
      PKBTestHelper::set_compare(expected_inverse, relationship_table->get(new VariableEntity("z"), true));

  int count = 0;
  int inverse_count = 0;
  int key_count = 0;
  int inverse_key_count = 0;
  EntityPointerUnorderedMap table = relationship_table->GetTable();
  EntityPointerUnorderedMap inverse_table = relationship_table->GetTable(true);

  for (auto &key : table) {
    count += key.second.size();
    key_count++;
  }

  for (auto &inverse_key : inverse_table) {
    inverse_count += inverse_key.second.size();
    inverse_key_count++;
  }

  ASSERT_EQ(count, 4);
  ASSERT_EQ(inverse_count, 4);
  ASSERT_EQ(key_count, 1);
  ASSERT_EQ(inverse_key_count, 4);
  ASSERT_EQ(compare, true);
  ASSERT_EQ(compare_inverse_w, true);
  ASSERT_EQ(compare_inverse_x, true);
  ASSERT_EQ(compare_inverse_y, true);
  ASSERT_EQ(compare_inverse_z, true);
}

/*
* Different procedures modify same set of variables 
*/
TEST(PopulateRelationshipTest, ModifiesMultipleProceduresTest) { 
  std::vector<std::pair<std::string, std::string>> modifies_one = {{"One", "w"},
                                                                   {"One", "x"},
                                                                   {"One", "y"},
                                                                   {"One", "z"}};
  std::vector<std::pair<std::string, std::string>> modifies_two = {{"Two", "w"},
                                                                   {"Two", "x"},
                                                                   {"Two", "y"},
                                                                   {"Two", "z"}};
  std::vector<std::pair<std::string, std::string>> modifies_three = {{"Three", "w"},
                                                                     {"Three", "x"},
                                                                     {"Three", "y"},
                                                                     {"Three", "z"}}; 
  std::vector<Relationship *> relationships;
  int recurse = 4;
  int length = modifies_one.size() + modifies_two.size() + modifies_three.size();
  relationships.reserve(length);
  EntityPointerUnorderedSet expected;
  EntityPointerUnorderedSet expected_inverse;
  expected.insert(new VariableEntity("w"));
  expected.insert(new VariableEntity("x"));
  expected.insert(new VariableEntity("y"));
  expected.insert(new VariableEntity("z"));
  expected_inverse.insert(new ProcedureEntity("One"));
  expected_inverse.insert(new ProcedureEntity("Two"));
  expected_inverse.insert(new ProcedureEntity("Three"));

  for (int i = 0; i < recurse; i++) {
    relationships.push_back(new ModifiesRelationship(new ProcedureEntity(modifies_one[i].first),
                                                     new VariableEntity(modifies_one[i].second)));
    relationships.push_back(new ModifiesRelationship(new ProcedureEntity(modifies_two[i].first),
                                                     new VariableEntity(modifies_two[i].second)));
    relationships.push_back(new ModifiesRelationship(new ProcedureEntity(modifies_three[i].first),
                                                     new VariableEntity(modifies_three[i].second))); 
  }

  auto *relationship_manager = new RelationshipManager();
  relationship_manager->Populate(relationships);
  auto *relationship_table = relationship_manager->GetTable(RsType::kModifies);
  bool compare_one = PKBTestHelper::set_compare(expected, relationship_table->get(new ProcedureEntity("One"), false));
  bool compare_two = PKBTestHelper::set_compare(expected, relationship_table->get(new ProcedureEntity("Two"), false));
  bool compare_three = PKBTestHelper::set_compare(expected, relationship_table->get(new ProcedureEntity("Three"), false)); 
  bool compare_inverse_w =
      PKBTestHelper::set_compare(expected_inverse, relationship_table->get(new VariableEntity("w"), true));
  bool compare_inverse_x =
      PKBTestHelper::set_compare(expected_inverse, relationship_table->get(new VariableEntity("x"), true));
  bool compare_inverse_y =
      PKBTestHelper::set_compare(expected_inverse, relationship_table->get(new VariableEntity("y"), true));
  bool compare_inverse_z =
      PKBTestHelper::set_compare(expected_inverse, relationship_table->get(new VariableEntity("z"), true));

  int count = 0;
  int inverse_count = 0;
  int key_count = 0;
  int inverse_key_count = 0;
  EntityPointerUnorderedMap table = relationship_table->GetTable();
  EntityPointerUnorderedMap inverse_table = relationship_table->GetTable(true);

  for (auto &key : table) {
    count += key.second.size();
    key_count++;
  }

  for (auto &inverse_key : inverse_table) {
    inverse_count += inverse_key.second.size();
    inverse_key_count++;
  }

  ASSERT_EQ(count, 12);
  ASSERT_EQ(inverse_count, 12);
  ASSERT_EQ(key_count, 3);
  ASSERT_EQ(inverse_key_count, 4);
  ASSERT_EQ(compare_one, true);
  ASSERT_EQ(compare_two, true);
  ASSERT_EQ(compare_three, true); 
  ASSERT_EQ(compare_inverse_w, true);
  ASSERT_EQ(compare_inverse_x, true);
  ASSERT_EQ(compare_inverse_y, true);
  ASSERT_EQ(compare_inverse_z, true);
}

/*
* Different statement numbers of the same type, each modifying a unique variable 
*/
TEST(PopulateRelationshipTest, ModifiesDifferentVariableTest) {
  std::vector<std::pair<std::string, std::string>> modifies = {{"1", "w"},
                                                               {"2", "z"},
                                                               {"3", "y"},
                                                               {"4", "x"}};
  std::vector<Relationship *> relationships;
  int length = modifies.size();
  relationships.reserve(length);
  EntityPointerUnorderedSet expected_1 = {new VariableEntity("w")};
  EntityPointerUnorderedSet expected_2 = {new VariableEntity("z")};
  EntityPointerUnorderedSet expected_3 = {new VariableEntity("y")};
  EntityPointerUnorderedSet expected_4 = {new VariableEntity("x")}; 
  EntityPointerUnorderedSet expected_inverse_w = {new AssignStmtEntity("1")};
  EntityPointerUnorderedSet expected_inverse_z = {new AssignStmtEntity("2")};
  EntityPointerUnorderedSet expected_inverse_y = {new AssignStmtEntity("3")};
  EntityPointerUnorderedSet expected_inverse_x = {new AssignStmtEntity("4")}; 

  for (int i = 0; i < length; i++) {
    relationships.push_back(
        new ModifiesRelationship(new AssignStmtEntity(modifies[i].first), 
                                 new VariableEntity(modifies[i].second)));
  }

  auto *relationship_manager = new RelationshipManager();
  relationship_manager->Populate(relationships);
  auto *relationship_table = relationship_manager->GetTable(RsType::kModifies);

  int count = 0;
  int key_count = 0;
  int inverse_count = 0;
  int inverse_key_count = 0;
  EntityPointerUnorderedMap table = relationship_table->GetTable();
  EntityPointerUnorderedMap inverse_table = relationship_table->GetTable(true);
  bool compare_1 = 
      PKBTestHelper::set_compare(expected_1, relationship_table->get(new AssignStmtEntity("1"), false));
  bool compare_2 = 
      PKBTestHelper::set_compare(expected_2, relationship_table->get(new AssignStmtEntity("2"), false));
  bool compare_3 = 
      PKBTestHelper::set_compare(expected_3, relationship_table->get(new AssignStmtEntity("3"), false));
  bool compare_4 = 
      PKBTestHelper::set_compare(expected_4, relationship_table->get(new AssignStmtEntity("4"), false));
  bool compare_inverse_w =
      PKBTestHelper::set_compare(expected_inverse_w, relationship_table->get(new VariableEntity("w"), true));
  bool compare_inverse_x =
      PKBTestHelper::set_compare(expected_inverse_z, relationship_table->get(new VariableEntity("z"), true));
  bool compare_inverse_y =
      PKBTestHelper::set_compare(expected_inverse_y, relationship_table->get(new VariableEntity("y"), true));
  bool compare_inverse_z =
      PKBTestHelper::set_compare(expected_inverse_x, relationship_table->get(new VariableEntity("x"), true));

  for (auto &key : table) { 
    count += key.second.size();
    key_count++;  
  }

  for (auto &inverse_key : inverse_table) { 
    inverse_count += inverse_key.second.size();
    inverse_key_count++; 
  } 

  ASSERT_EQ(count, 4);
  ASSERT_EQ(inverse_count, 4);
  ASSERT_EQ(key_count, 4);
  ASSERT_EQ(inverse_key_count, 4);
  ASSERT_EQ(compare_1, true);
  ASSERT_EQ(compare_2, true);
  ASSERT_EQ(compare_3, true);
  ASSERT_EQ(compare_4, true);
  ASSERT_EQ(compare_inverse_w, true);
  ASSERT_EQ(compare_inverse_x, true);
  ASSERT_EQ(compare_inverse_y, true);
  ASSERT_EQ(compare_inverse_z, true); 
}

/*
* Different statement numbers of the same type, each modifying the same variable 
*/
TEST(PopulateRelationshipTest, ModifiesSingleVariableTest) { 
  std::vector<std::pair<std::string, std::string>> modifies = {{"1", "w"},
                                                               {"2", "w"},
                                                               {"3", "w"},
                                                               {"4", "w"}};
  std::vector<Relationship *> relationships;
  int length = modifies.size();
  relationships.reserve(length);
  EntityPointerUnorderedSet expected = {new VariableEntity("w")}; 
  EntityPointerUnorderedSet expected_inverse = {new AssignStmtEntity("1"), new AssignStmtEntity("2"),
                                                  new AssignStmtEntity("3"), new AssignStmtEntity("4")}; 

  for (int i = 0; i < length; i++) {
    relationships.push_back(
        new ModifiesRelationship(new AssignStmtEntity(modifies[i].first), 
                                 new VariableEntity(modifies[i].second)));
  }

  auto *relationship_manager = new RelationshipManager();
  relationship_manager->Populate(relationships);
  auto *relationship_table = relationship_manager->GetTable(RsType::kModifies);

  int count = 0;
  int key_count = 0;
  int inverse_count = 0;
  int inverse_key_count = 0;
  EntityPointerUnorderedMap table = relationship_table->GetTable();
  EntityPointerUnorderedMap inverse_table = relationship_table->GetTable(true);
  bool compare_1 = 
      PKBTestHelper::set_compare(expected, relationship_table->get(new AssignStmtEntity("1"), false));
  bool compare_2 = 
      PKBTestHelper::set_compare(expected, relationship_table->get(new AssignStmtEntity("2"), false));
  bool compare_3 = 
      PKBTestHelper::set_compare(expected, relationship_table->get(new AssignStmtEntity("3"), false));
  bool compare_4 = 
      PKBTestHelper::set_compare(expected, relationship_table->get(new AssignStmtEntity("4"), false)); 
  bool compare_inverse =
      PKBTestHelper::set_compare(expected_inverse, relationship_table->get(new VariableEntity("w"), true));

  for (auto &key : table) {
    count += key.second.size();
    key_count++;
  }

  for (auto &inverse_key : inverse_table) {
    inverse_count += inverse_key.second.size();
    inverse_key_count++;
  }

  ASSERT_EQ(count, 4);
  ASSERT_EQ(inverse_count, 4);
  ASSERT_EQ(key_count, 4);
  ASSERT_EQ(inverse_key_count, 1);
  ASSERT_EQ(compare_1, true);
  ASSERT_EQ(compare_2, true);
  ASSERT_EQ(compare_3, true);
  ASSERT_EQ(compare_4, true);
  ASSERT_EQ(compare_inverse, true);
}

/*
* Different statement types and numbers, each modifying a unique variable 
*/
TEST(PopulateRelationshipTest, ModifiesMultipleTypeStatementsTest) {
  std::vector<std::pair<std::string, std::string>> modifies_assign = {{"1", "w"},
                                                                      {"2", "x"}, 
                                                                      {"3", "y"}, 
                                                                      {"4", "z"}};
  std::vector<std::pair<std::string, std::string>> modifies_read = {{"5", "a"}, 
                                                                    {"6", "b"},
                                                                    {"7", "c"},
                                                                    {"8", "d"}};
  std::vector<std::pair<std::string, std::string>> modifies_if = {{"10", "p"},
                                                                  {"15", "q"},
                                                                  {"20", "r"},
                                                                  {"25", "s"}};
  std::vector<Relationship *> relationships;
  int recurse = 4;
  int length = modifies_assign.size() + modifies_read.size() + modifies_if.size();
  relationships.reserve(length);
  EntityPointerUnorderedSet actual;
  EntityPointerUnorderedSet actual_inverse;
  EntityPointerUnorderedSet expected; 
  EntityPointerUnorderedSet expected_inverse; 

  for (int i = 0; i < recurse; i++) {
    relationships.push_back(new ModifiesRelationship(new AssignStmtEntity(modifies_assign[i].first),
                                                     new VariableEntity(modifies_assign[i].second)));
    relationships.push_back(new ModifiesRelationship(new ReadStmtEntity(modifies_read[i].first),
                                                     new VariableEntity(modifies_read[i].second)));
    relationships.push_back(new ModifiesRelationship(new IfStmtEntity(modifies_if[i].first),
                                                     new VariableEntity(modifies_if[i].second)));
    expected.insert(new VariableEntity(modifies_assign[i].second));
    expected.insert(new VariableEntity(modifies_read[i].second));
    expected.insert(new VariableEntity(modifies_if[i].second));
    expected_inverse.insert(new AssignStmtEntity(modifies_assign[i].first)); 
    expected_inverse.insert(new ReadStmtEntity(modifies_read[i].first));
    expected_inverse.insert(new IfStmtEntity(modifies_if[i].first)); 
  }

  auto *relationship_manager = new RelationshipManager();
  relationship_manager->Populate(relationships);
  auto *relationship_table = relationship_manager->GetTable(RsType::kModifies);

  int count = 0;
  int key_count = 0;
  int inverse_count = 0;
  int inverse_key_count = 0; 
  EntityPointerUnorderedMap table = relationship_table->GetTable();
  EntityPointerUnorderedMap inverse_table = relationship_table->GetTable(true);

  for (auto &key : table) {
    count += key.second.size();
    actual.insert(key.second.begin(), key.second.end());
    key_count++;
  }

  for (auto &inverse_key : inverse_table) {
    inverse_count += inverse_key.second.size();
    actual_inverse.insert(inverse_key.second.begin(), inverse_key.second.end());
    inverse_key_count++; 
  }

  bool compare = PKBTestHelper::set_compare(expected, actual);
  bool compare_inverse = PKBTestHelper::set_compare(expected_inverse, actual_inverse);

  ASSERT_EQ(count, 12);
  ASSERT_EQ(inverse_count, 12);
  ASSERT_EQ(key_count, 12);
  ASSERT_EQ(inverse_key_count, 12);
  ASSERT_EQ(compare, true);
  ASSERT_EQ(compare_inverse, true);
}

/*
* Different statements and procedures, modifying the same set of variables
*/
TEST(PopulateRelationshipTest, ModifiesMixedTest) {
  std::vector<std::pair<std::string, std::string>> modifies_stmt = {{"1", "w"},
                                                                    {"2", "x"},
                                                                    {"3", "y"},
                                                                    {"4", "z"}};
  std::vector<std::pair<std::string, std::string>> modifies_procedure = {{"One", "w"},
                                                                         {"Two", "x"},
                                                                         {"Three", "y"},
                                                                         {"Four", "z"}};
  std::vector<Relationship *> relationships;
  int recurse = 4;
  int length = modifies_stmt.size() + modifies_procedure.size();
  relationships.reserve(length);
  EntityPointerUnorderedSet actual;
  EntityPointerUnorderedSet actual_inverse;
  EntityPointerUnorderedSet expected;
  EntityPointerUnorderedSet expected_inverse;

  for (int i = 0; i < recurse; i++) {
    relationships.push_back(new ModifiesRelationship(new AssignStmtEntity(modifies_stmt[i].first),
                                                     new VariableEntity(modifies_stmt[i].second)));
    relationships.push_back(new ModifiesRelationship(new ProcedureEntity(modifies_procedure[i].first),
                                                     new VariableEntity(modifies_procedure[i].second)));
    expected.insert(new VariableEntity(modifies_stmt[i].second));
    expected.insert(new VariableEntity(modifies_procedure[i].second));
    expected_inverse.insert(new AssignStmtEntity(modifies_stmt[i].first));
    expected_inverse.insert(new ProcedureEntity(modifies_procedure[i].first));
  }

  auto *relationship_manager = new RelationshipManager();
  relationship_manager->Populate(relationships);
  auto *relationship_table = relationship_manager->GetTable(RsType::kModifies);

  int count = 0;
  int key_count = 0;
  int inverse_count = 0;
  int inverse_key_count = 0;
  EntityPointerUnorderedMap table = relationship_table->GetTable();
  EntityPointerUnorderedMap inverse_table = relationship_table->GetTable(true);

  for (auto &key : table) {
    count += key.second.size();
    actual.insert(key.second.begin(), key.second.end()); 
    key_count++;
  }

  for (auto &inverse_key : inverse_table) {
    inverse_count += inverse_key.second.size();
    actual_inverse.insert(inverse_key.second.begin(), inverse_key.second.end()); 
    inverse_key_count++;
  }

  bool compare = PKBTestHelper::set_compare(expected, actual);
  bool compare_inverse = PKBTestHelper::set_compare(expected_inverse, actual_inverse);

  ASSERT_EQ(count, 8);
  ASSERT_EQ(inverse_count, 8); 
  ASSERT_EQ(key_count, 8);
  ASSERT_EQ(inverse_key_count, 4);
  ASSERT_EQ(compare, true);
  ASSERT_EQ(compare_inverse, true);
}

/*
* Relationship between statements of the same type
*/
TEST(PopulateRelationshipTest, FollowsSingleStatementTypeTest) {
  std::vector<std::pair<std::string, std::string>> follows = {{"1", "2"}, 
                                                              {"2", "3"}, 
                                                              {"3", "4"}, 
                                                              {"4", "5"}};
  std::vector<Relationship *> relationships;
  int length = follows.size();
  relationships.reserve(length);
  EntityPointerUnorderedSet actual;
  EntityPointerUnorderedSet actual_inverse;
  EntityPointerUnorderedSet expected;
  EntityPointerUnorderedSet expected_inverse;

  for (int i = 0; i < length; i++) {
    relationships.push_back(
        new FollowsRelationship(new AssignStmtEntity(follows[i].first), 
                                new AssignStmtEntity(follows[i].second)));
    expected.insert(new AssignStmtEntity(follows[i].second)); 
    expected_inverse.insert(new AssignStmtEntity(follows[i].first)); 
  }

  auto *relationship_manager = new RelationshipManager();
  relationship_manager->Populate(relationships);
  auto *relationship_table = relationship_manager->GetTable(RsType::kFollows);

  int count = 0;
  int inverse_count = 0;
  int key_count = 0;
  int inverse_key_count = 0;
  EntityPointerUnorderedMap table = relationship_table->GetTable();
  EntityPointerUnorderedMap inverse_table = relationship_table->GetTable(true);

  for (auto &key : table) {
    count += key.second.size();
    actual.insert(key.second.begin(), key.second.end()); 
    key_count++;
  }

  for (auto &inverse_key : inverse_table) {
    inverse_count += inverse_key.second.size();
    actual_inverse.insert(inverse_key.second.begin(), inverse_key.second.end()); 
    inverse_key_count++;
  }

  bool compare = PKBTestHelper::set_compare(expected, actual);
  bool compare_inverse = PKBTestHelper::set_compare(expected_inverse, actual_inverse);

  ASSERT_EQ(count, 4);
  ASSERT_EQ(inverse_count, 4);
  ASSERT_EQ(key_count, 4);
  ASSERT_EQ(inverse_key_count, 4);
  ASSERT_EQ(compare, true);
  ASSERT_EQ(compare_inverse, true);
}

/*
* Relationship between statements of different types
*/
TEST(PopulateRelationshipTest, FollowsMultipleStatementTypeTest) {
  std::vector<std::pair<std::string, std::string>> follows_a = {{"1", "2"}, 
                                                                {"2", "3"}, 
                                                                {"3", "4"}, 
                                                                {"4", "5"}};  
  std::vector<std::pair<std::string, std::string>> follows_b = {{"10", "11"}, 
                                                                {"15", "16"}, 
                                                                {"20", "21"}, 
                                                                {"25", "26"}}; 
  std::vector<Relationship *> relationships;
  int recurse = 4;
  int length = follows_a.size() + follows_b.size();
  relationships.reserve(length);
  EntityPointerUnorderedSet actual;
  EntityPointerUnorderedSet actual_inverse;
  EntityPointerUnorderedSet expected;
  EntityPointerUnorderedSet expected_inverse;

  for (int i = 0; i < recurse; i++) {
    relationships.push_back(
        new FollowsRelationship(new AssignStmtEntity(follows_a[i].first), 
                                new ReadStmtEntity(follows_a[i].second)));
    relationships.push_back(
        new FollowsRelationship(new IfStmtEntity(follows_b[i].first), 
                                new AssignStmtEntity(follows_b[i].second)));
    expected.insert(new ReadStmtEntity(follows_a[i].second));
    expected.insert(new AssignStmtEntity(follows_b[i].second));
    expected_inverse.insert(new AssignStmtEntity(follows_a[i].first));
    expected_inverse.insert(new IfStmtEntity(follows_b[i].first)); 
  }

  auto *relationship_manager = new RelationshipManager();
  relationship_manager->Populate(relationships);
  auto *relationship_table = relationship_manager->GetTable(RsType::kFollows);

  int count = 0;
  int inverse_count = 0;
  int key_count = 0;
  int inverse_key_count = 0;
  EntityPointerUnorderedMap table = relationship_table->GetTable();
  EntityPointerUnorderedMap inverse_table = relationship_table->GetTable(true);

  for (auto &key : table) {
    count += key.second.size();
    actual.insert(key.second.begin(), key.second.end()); 
    key_count++;
  }

  for (auto &inverse_key : inverse_table) {
    inverse_count += inverse_key.second.size();
    actual_inverse.insert(inverse_key.second.begin(), inverse_key.second.end()); 
    inverse_key_count++;
  }

  bool compare = PKBTestHelper::set_compare(expected, actual);
  bool compare_inverse = PKBTestHelper::set_compare(expected_inverse, actual_inverse);
   
  ASSERT_EQ(count, 8);
  ASSERT_EQ(inverse_count, 8);
  ASSERT_EQ(key_count, 8);
  ASSERT_EQ(inverse_key_count, 8);
  ASSERT_EQ(compare, true);
  ASSERT_EQ(compare_inverse, true);
}
  
/*
* Same procedure calling multiple procedures
*/
TEST(PopulateRelationshipTest, CallsMultipleProcedureTest) {
  std::vector<std::pair<std::string, std::string>> calls = {{"One", "Two"},
                                                            {"One", "Three"},
                                                            {"One", "Four"},
                                                            {"One", "Five"}};
  std::vector<Relationship *> relationships;
  int length = calls.size();
  relationships.reserve(length);
  EntityPointerUnorderedSet actual;
  EntityPointerUnorderedSet actual_inverse;
  EntityPointerUnorderedSet expected;
  EntityPointerUnorderedSet expected_inverse;

  for (int i = 0; i < length; i++) {
    relationships.push_back(
        new CallsRelationship(new ProcedureEntity(calls[i].first), 
                              new ProcedureEntity(calls[i].second)));
    expected.insert(new ProcedureEntity(calls[i].second));
    expected_inverse.insert(new ProcedureEntity(calls[i].first));
  }

  auto *relationship_manager = new RelationshipManager();
  relationship_manager->Populate(relationships);
  auto *relationship_table = relationship_manager->GetTable(RsType::kCalls);

  int count = 0;
  int inverse_count = 0;
  int key_count = 0;
  int inverse_key_count = 0;
  EntityPointerUnorderedMap table = relationship_table->GetTable();
  EntityPointerUnorderedMap inverse_table = relationship_table->GetTable(true);

  for (auto &key : table) {
    count += key.second.size();
    actual.insert(key.second.begin(), key.second.end());  
    key_count++;
  }

  for (auto &inverse_key : inverse_table) {
    inverse_count += inverse_key.second.size();
    actual_inverse.insert(inverse_key.second.begin(), inverse_key.second.end());  
    inverse_key_count++;
  }

  bool compare = PKBTestHelper::set_compare(expected, actual);
  bool compare_inverse = PKBTestHelper::set_compare(expected_inverse, actual_inverse);

  ASSERT_EQ(count, 4);
  ASSERT_EQ(inverse_count, 4);
  ASSERT_EQ(key_count, 1);
  ASSERT_EQ(inverse_key_count, 4);
  ASSERT_EQ(compare, true);
  ASSERT_EQ(compare_inverse, true);
}

/*
* Multiple procedures calling the same procedure
*/
TEST(PopulateRelationshipTest, CallsSameProcedureTest) {
  std::vector<std::pair<std::string, std::string>> calls = {{"One", "Five"},
                                                            {"Two", "Five"},
                                                            {"Three", "Five"},
                                                            {"Four", "Five"}};
  std::vector<Relationship *> relationships;
  int length = calls.size();
  relationships.reserve(length);
  EntityPointerUnorderedSet actual;
  EntityPointerUnorderedSet actual_inverse;
  EntityPointerUnorderedSet expected;
  EntityPointerUnorderedSet expected_inverse;

  for (int i = 0; i < length; i++) {
    relationships.push_back(
        new CallsRelationship(new ProcedureEntity(calls[i].first), 
                              new ProcedureEntity(calls[i].second)));
    expected.insert(new ProcedureEntity(calls[i].second));
    expected_inverse.insert(new ProcedureEntity(calls[i].first));
  }

  auto *relationship_manager = new RelationshipManager();
  relationship_manager->Populate(relationships);
  auto *relationship_table = relationship_manager->GetTable(RsType::kCalls);

  int count = 0;
  int inverse_count = 0;
  int key_count = 0;
  int inverse_key_count = 0;
  EntityPointerUnorderedMap table = relationship_table->GetTable();
  EntityPointerUnorderedMap inverse_table = relationship_table->GetTable(true);

  for (auto &key : table) {
    count += key.second.size();
    key_count++;
  }

  for (auto &inverse_key : inverse_table) {
    inverse_count += inverse_key.second.size();
    inverse_key_count++;
  }

  ASSERT_EQ(count, 4);
  ASSERT_EQ(inverse_count, 4);
  ASSERT_EQ(key_count, 4);
  ASSERT_EQ(inverse_key_count, 1);
}

/*
* Multiple procedures calling multiple unique procedures
*/
TEST(PopulateRelationshipTest, CallsMultipleUniqueProcedureTest) {
  std::vector<std::pair<std::string, std::string>> calls = {{"One", "Five"},
                                                            {"Two", "Six"},
                                                            {"Three", "Seven"},
                                                            {"Four", "Eight"}};
  std::vector<Relationship *> relationships;
  int length = calls.size();
  relationships.reserve(length);
  EntityPointerUnorderedSet actual;
  EntityPointerUnorderedSet actual_inverse;
  EntityPointerUnorderedSet expected;
  EntityPointerUnorderedSet expected_inverse;

  for (int i = 0; i < length; i++) {
    relationships.push_back(
        new CallsRelationship(new ProcedureEntity(calls[i].first), 
                              new ProcedureEntity(calls[i].second)));
    expected.insert(new ProcedureEntity(calls[i].second));
    expected_inverse.insert(new ProcedureEntity(calls[i].first));
  }

  auto *relationship_manager = new RelationshipManager();
  relationship_manager->Populate(relationships);
  auto *relationship_table = relationship_manager->GetTable(RsType::kCalls);

  int count = 0;
  int inverse_count = 0;
  int key_count = 0;
  int inverse_key_count = 0;
  EntityPointerUnorderedMap table = relationship_table->GetTable();
  EntityPointerUnorderedMap inverse_table = relationship_table->GetTable(true);

  for (auto &key : table) {
    count += key.second.size();
    actual.insert(key.second.begin(), key.second.end());  
    key_count++;
  }

  for (auto &inverse_key : inverse_table) {
    inverse_count += inverse_key.second.size();
    actual_inverse.insert(inverse_key.second.begin(), inverse_key.second.end());  
    inverse_key_count++;
  }
  
  bool compare = PKBTestHelper::set_compare(expected, actual);
  bool compare_inverse = PKBTestHelper::set_compare(expected_inverse, actual_inverse);

  ASSERT_EQ(count, 4);
  ASSERT_EQ(inverse_count, 4);
  ASSERT_EQ(key_count, 4);
  ASSERT_EQ(inverse_key_count, 4);
  ASSERT_EQ(compare, true);
  ASSERT_EQ(compare_inverse, true);
}

/*
* Multiple duplicates of a single relationship (Modifies)
*/
TEST(PopulateRelationshipTest, DuplicateSingleRelationshipTest) {
  std::vector<std::pair<std::string, std::string>> modifies = {{"1", "x"}, {"1", "x"}, {"1", "x"}};
  std::vector<Relationship *> relationships;
  int length = modifies.size();
  relationships.reserve(length); 
  EntityPointerUnorderedSet expected = {new VariableEntity("x")};
  EntityPointerUnorderedSet expected_inverse = {new AssignStmtEntity("1")};

  for (int i = 0; i < length; i++) {
    relationships.push_back(
        new ModifiesRelationship(new AssignStmtEntity(modifies[i].first), 
                                 new VariableEntity(modifies[i].second)));
  }

  auto *relationship_manager = new RelationshipManager();
  relationship_manager->Populate(relationships);
  auto *relationship_table = relationship_manager->GetTable(RsType::kModifies);

  int count = 0;
  int inverse_count = 0;
  int key_count = 0;
  int inverse_key_count = 0;
  EntityPointerUnorderedMap table = relationship_table->GetTable();
  EntityPointerUnorderedMap inverse_table = relationship_table->GetTable(true);
  bool compare = 
      PKBTestHelper::set_compare(expected, relationship_table->get(new AssignStmtEntity("1"), false));
  bool compare_inverse = 
      PKBTestHelper::set_compare(expected_inverse, relationship_table->get(new VariableEntity("x"), true));

  for (auto &key : table) {
    count += key.second.size();
    key_count++;
  }

  for (auto &inverse_key : inverse_table) {
    inverse_count += inverse_key.second.size();
    inverse_key_count++;
  }

  ASSERT_EQ(count, 1);
  ASSERT_EQ(inverse_count, 1);
  ASSERT_EQ(key_count, 1);
  ASSERT_EQ(inverse_key_count, 1);
  ASSERT_EQ(compare, true);
  ASSERT_EQ(compare_inverse, true);
}

/*
* Stress test for a single relationship (Follows)
*/
TEST(PopulateRelationshipTest, SingleRelationshipStressTest) {
  std::vector<Relationship *> relationships;
  int length = 1000;
  relationships.reserve(length);
  EntityPointerUnorderedSet actual;
  EntityPointerUnorderedSet actual_inverse;
  EntityPointerUnorderedSet expected;
  EntityPointerUnorderedSet expected_inverse;

  for (int i = 0; i < length; i = i + 2) {
    std::string stmt_a = std::to_string(i);
    std::string stmt_b = std::to_string(i + 1);
    relationships.push_back(new FollowsRelationship(new AssignStmtEntity(stmt_a), 
                                                    new AssignStmtEntity(stmt_b)));
    expected.insert(new AssignStmtEntity(stmt_b));
    expected_inverse.insert(new AssignStmtEntity(stmt_a));
  }

  auto *relationship_manager = new RelationshipManager();
  relationship_manager->Populate(relationships);
  auto *relationship_table = relationship_manager->GetTable(RsType::kFollows);

  int count = 0;
  int inverse_count = 0;
  int key_count = 0;
  int inverse_key_count = 0;
  EntityPointerUnorderedMap table = relationship_table->GetTable();
  EntityPointerUnorderedMap inverse_table = relationship_table->GetTable(true);

  for (auto &key : table) {
    count += key.second.size();
    actual.insert(key.second.begin(), key.second.end());   
    key_count++;
  }

  for (auto &inverse_key : inverse_table) {
    inverse_count += inverse_key.second.size();
    actual_inverse.insert(inverse_key.second.begin(), inverse_key.second.end());   
    inverse_key_count++;
  }
  
  bool compare = PKBTestHelper::set_compare(expected, actual);
  bool compare_inverse = PKBTestHelper::set_compare(expected_inverse, actual_inverse);

  ASSERT_EQ(count, 500);
  ASSERT_EQ(inverse_count, 500);
  ASSERT_EQ(key_count, 500);
  ASSERT_EQ(inverse_key_count, 500);
  ASSERT_EQ(compare, true);
  ASSERT_EQ(compare_inverse, true);
}

/*
* Multiple relationships with no duplicates
*/
TEST(PopulateRelationshipTest, MultipleRelationshipsTest) {
  std::vector<std::pair<std::string, std::string>> modifies = {{"1", "w"},
                                                               {"2", "z"},
                                                               {"3", "y"},
                                                               {"4", "x"}};
  std::vector<std::pair<std::string, std::string>> follows = {{"1", "2"}, 
                                                              {"2", "3"}, 
                                                              {"3", "4"}, 
                                                              {"4", "5"}};
  std::vector<std::pair<std::string, std::string>> calls = {{"One", "Five"},
                                                            {"Two", "Six"},
                                                            {"Three", "Seven"},
                                                            {"Four", "Eight"}};
  std::vector<Relationship *> relationships;
  int recurse = 4;
  int length = calls.size() + modifies.size() + follows.size();
  relationships.reserve(length);
  EntityPointerUnorderedSet actual_modifies;
  EntityPointerUnorderedSet actual_modifies_inverse;
  EntityPointerUnorderedSet expected_modifies;
  EntityPointerUnorderedSet expected_modifies_inverse;
  EntityPointerUnorderedSet actual_follows;
  EntityPointerUnorderedSet actual_follows_inverse;
  EntityPointerUnorderedSet expected_follows;
  EntityPointerUnorderedSet expected_follows_inverse;
  EntityPointerUnorderedSet actual_calls;
  EntityPointerUnorderedSet actual_calls_inverse;
  EntityPointerUnorderedSet expected_calls;
  EntityPointerUnorderedSet expected_calls_inverse;

  for (int i = 0; i < recurse; i++) {
    relationships.push_back(
        new ModifiesRelationship(new AssignStmtEntity(modifies[i].first), 
                                 new VariableEntity(modifies[i].second)));
    relationships.push_back(
        new FollowsRelationship(new AssignStmtEntity(follows[i].first), 
                                new ReadStmtEntity(follows[i].second)));
    relationships.push_back(
        new CallsRelationship(new ProcedureEntity(calls[i].first), 
                              new ProcedureEntity(calls[i].second)));
    expected_modifies.insert(new VariableEntity(modifies[i].second));
    expected_follows.insert(new ReadStmtEntity(follows[i].second));
    expected_calls.insert(new ProcedureEntity(calls[i].second)); 
    expected_modifies_inverse.insert(new AssignStmtEntity(modifies[i].first));
    expected_follows_inverse.insert(new AssignStmtEntity(follows[i].first));
    expected_calls_inverse.insert(new ProcedureEntity(calls[i].first));
  }

  auto *relationship_manager = new RelationshipManager();
  relationship_manager->Populate(relationships);
  auto *modifies_table = relationship_manager->GetTable(RsType::kModifies);
  auto *follows_table = relationship_manager->GetTable(RsType::kFollows);
  auto *calls_table = relationship_manager->GetTable(RsType::kCalls);

  int modifies_count = 0;
  int inverse_modifies_count = 0;
  int modifies_key_count = 0;
  int inverse_modifies_key_count = 0;
  int follows_count = 0;
  int inverse_follows_count = 0;
  int follows_key_count = 0;
  int inverse_follows_key_count = 0;
  int calls_count = 0;
  int inverse_calls_count = 0;
  int calls_key_count = 0;
  int inverse_calls_key_count = 0;
  EntityPointerUnorderedMap modifies_map = modifies_table->GetTable();
  EntityPointerUnorderedMap inverse_modifies_map = modifies_table->GetTable(true);
  EntityPointerUnorderedMap follows_map = follows_table->GetTable();
  EntityPointerUnorderedMap inverse_follows_map = follows_table->GetTable(true);
  EntityPointerUnorderedMap calls_map = calls_table->GetTable();
  EntityPointerUnorderedMap inverse_calls_map = calls_table->GetTable(true);

  for (auto &key : modifies_map) {
    modifies_count += key.second.size();
    actual_modifies.insert(key.second.begin(), key.second.end());    
    modifies_key_count++;
  }
  for (auto &inverse_key : inverse_modifies_map) {
    inverse_modifies_count += inverse_key.second.size();
    actual_modifies_inverse.insert(inverse_key.second.begin(), inverse_key.second.end());   
    inverse_modifies_key_count++;
  }
  for (auto &key : follows_map) {
    follows_count += key.second.size();
    actual_follows.insert(key.second.begin(), key.second.end());  
    follows_key_count++;
  }
  for (auto &inverse_key : inverse_follows_map) {
    inverse_follows_count += inverse_key.second.size();
    actual_follows_inverse.insert(inverse_key.second.begin(), inverse_key.second.end());   
    inverse_follows_key_count++;
  }
  for (auto &key : calls_map) {
    calls_count += key.second.size();
    actual_calls.insert(key.second.begin(), key.second.end());    
    calls_key_count++;
  }
  for (auto &inverse_key : inverse_calls_map) {
    inverse_calls_count += inverse_key.second.size();
    actual_calls_inverse.insert(inverse_key.second.begin(), inverse_key.second.end());    
    inverse_calls_key_count++;
  }

  bool compare_modifies = PKBTestHelper::set_compare(expected_modifies, actual_modifies);
  bool compare_modifies_inverse = PKBTestHelper::set_compare(expected_modifies_inverse, actual_modifies_inverse);
  bool compare_follows = PKBTestHelper::set_compare(expected_follows, actual_follows);
  bool compare_follows_inverse = PKBTestHelper::set_compare(expected_follows_inverse, actual_follows_inverse);
  bool compare_calls = PKBTestHelper::set_compare(expected_calls, actual_calls);
  bool compare_calls_inverse = PKBTestHelper::set_compare(expected_calls_inverse, actual_calls_inverse);

  ASSERT_EQ(modifies_count, 4);
  ASSERT_EQ(modifies_key_count, 4);
  ASSERT_EQ(inverse_modifies_count, 4);
  ASSERT_EQ(inverse_modifies_key_count, 4);
  ASSERT_EQ(compare_modifies, true);
  ASSERT_EQ(compare_modifies_inverse, true); 

  ASSERT_EQ(follows_count, 4);
  ASSERT_EQ(follows_key_count, 4);
  ASSERT_EQ(inverse_follows_count, 4);
  ASSERT_EQ(inverse_follows_key_count, 4);
  ASSERT_EQ(compare_follows, true);
  ASSERT_EQ(compare_follows_inverse, true); 

  ASSERT_EQ(calls_count, 4);
  ASSERT_EQ(calls_key_count, 4);
  ASSERT_EQ(inverse_calls_count, 4);
  ASSERT_EQ(inverse_calls_key_count, 4);
  ASSERT_EQ(compare_calls, true);
  ASSERT_EQ(compare_calls_inverse, true);
}

/*
* Multiple duplicates of multiple relationships
*/
TEST(PopulatieRelationshipTest, DuplicateMultipleRelationshipTest) {
  std::vector<std::pair<std::string, std::string>> modifies = {{"1", "w"},
                                                               {"1", "w"}, 
                                                               {"1", "w"}};
  std::vector<std::pair<std::string, std::string>> follows = {{"2", "3"}, 
                                                              {"2", "3"},  
                                                              {"2", "3"}};
  std::vector<std::pair<std::string, std::string>> calls = {{"One", "Two"},
                                                            {"One", "Two"}, 
                                                            {"One", "Two"}};
  std::vector<Relationship *> relationships;
  int recurse = 3;
  int length = modifies.size() + follows.size() + calls.size();
  relationships.reserve(length); 
  EntityPointerUnorderedSet expected_modifies = {new VariableEntity("w")};
  EntityPointerUnorderedSet expected_modifies_inverse = {new AssignStmtEntity("1")}; 
  EntityPointerUnorderedSet expected_follows = {new ReadStmtEntity("3")};
  EntityPointerUnorderedSet expected_follows_inverse = {new AssignStmtEntity("2")}; 
  EntityPointerUnorderedSet expected_calls = {new ProcedureEntity("Two")};
  EntityPointerUnorderedSet expected_calls_inverse = {new ProcedureEntity("One")};

  for (int i = 0; i < recurse; i++) {
    relationships.push_back(
        new ModifiesRelationship(new AssignStmtEntity(modifies[i].first), 
                                 new VariableEntity(modifies[i].second)));
    relationships.push_back(
        new FollowsRelationship(new AssignStmtEntity(follows[i].first), 
                                new ReadStmtEntity(follows[i].second)));
    relationships.push_back(
        new CallsRelationship(new ProcedureEntity(calls[i].first), 
                              new ProcedureEntity(calls[i].second)));
  }

  auto *relationship_manager = new RelationshipManager();
  relationship_manager->Populate(relationships);
  auto *modifies_table = relationship_manager->GetTable(RsType::kModifies);
  auto *follows_table = relationship_manager->GetTable(RsType::kFollows);
  auto *calls_table = relationship_manager->GetTable(RsType::kCalls);

  int modifies_count = 0;
  int inverse_modifies_count = 0;
  int modifies_key_count = 0;
  int inverse_modifies_key_count = 0;
  int follows_count = 0;
  int inverse_follows_count = 0;
  int follows_key_count = 0;
  int inverse_follows_key_count = 0;
  int calls_count = 0;
  int inverse_calls_count = 0;
  int calls_key_count = 0;
  int inverse_calls_key_count = 0;
  EntityPointerUnorderedMap modifies_map = modifies_table->GetTable();
  EntityPointerUnorderedMap inverse_modifies_map = modifies_table->GetTable(true);
  EntityPointerUnorderedMap follows_map = follows_table->GetTable();
  EntityPointerUnorderedMap inverse_follows_map = follows_table->GetTable(true);
  EntityPointerUnorderedMap calls_map = calls_table->GetTable();
  EntityPointerUnorderedMap inverse_calls_map = calls_table->GetTable(true);
  bool compare_modifies =
      PKBTestHelper::set_compare(expected_modifies, modifies_table->get(new AssignStmtEntity("1"), false));
  bool compare_modifies_inverse =
      PKBTestHelper::set_compare(expected_modifies_inverse, modifies_table->get(new VariableEntity("w"), true));
  bool compare_follows =
      PKBTestHelper::set_compare(expected_follows, follows_table->get(new AssignStmtEntity("2"), false));
  bool compare_follows_inverse =
      PKBTestHelper::set_compare(expected_follows_inverse, follows_table->get(new ReadStmtEntity("3"), true));
  bool compare_calls =
      PKBTestHelper::set_compare(expected_calls, calls_table->get(new ProcedureEntity("One"), false));
  bool compare_calls_inverse =
      PKBTestHelper::set_compare(expected_calls_inverse, calls_table->get(new ProcedureEntity("Two"), true));

  for (auto &key : modifies_map) {
    modifies_count += key.second.size();
    modifies_key_count++;
  }
  for (auto &inverse_key : inverse_modifies_map) {
    inverse_modifies_count += inverse_key.second.size();
    inverse_modifies_key_count++;
  }
  for (auto &key : follows_map) {
    follows_count += key.second.size();
    follows_key_count++;
  }
  for (auto &inverse_key : inverse_follows_map) {
    inverse_follows_count += inverse_key.second.size();
    inverse_follows_key_count++;
  }
  for (auto &key : calls_map) {
    calls_count += key.second.size();
    calls_key_count++;
  }
  for (auto &inverse_key : inverse_calls_map) {
    inverse_calls_count += inverse_key.second.size();
    inverse_calls_key_count++;
  }

  ASSERT_EQ(modifies_count, 1);
  ASSERT_EQ(modifies_key_count, 1);
  ASSERT_EQ(inverse_modifies_count, 1);
  ASSERT_EQ(inverse_modifies_key_count, 1);
  ASSERT_EQ(compare_modifies, true);
  ASSERT_EQ(compare_modifies_inverse, true); 

  ASSERT_EQ(follows_count, 1);
  ASSERT_EQ(follows_key_count, 1);
  ASSERT_EQ(inverse_follows_count, 1);
  ASSERT_EQ(inverse_follows_key_count, 1);
  ASSERT_EQ(compare_follows, true);
  ASSERT_EQ(compare_follows_inverse, true); 

  ASSERT_EQ(calls_count, 1);
  ASSERT_EQ(calls_key_count, 1);
  ASSERT_EQ(inverse_calls_count, 1);
  ASSERT_EQ(inverse_calls_key_count, 1);
  ASSERT_EQ(compare_calls, true);
  ASSERT_EQ(compare_calls_inverse, true);
}

/*
* Stress test for multiple relationships
*/
TEST(PopulateRelationshipTest, MultipleRelationshipStressTest) {
  std::vector<Relationship *> relationships;
  int recurse = 1000;
  int length = 1000;
  relationships.reserve(length);
  EntityPointerUnorderedSet actual_modifies;
  EntityPointerUnorderedSet actual_modifies_inverse;
  EntityPointerUnorderedSet expected_modifies;
  EntityPointerUnorderedSet expected_modifies_inverse;
  EntityPointerUnorderedSet actual_follows;
  EntityPointerUnorderedSet actual_follows_inverse;
  EntityPointerUnorderedSet expected_follows;
  EntityPointerUnorderedSet expected_follows_inverse; 

  for (int i = 0; i < recurse; i = i + 2) {
    std::string stmt_a = std::to_string(i); 
    std::string stmt_b = std::to_string(i + 1); 
    relationships.push_back(
        new ModifiesRelationship(new AssignStmtEntity(stmt_a), 
                                 new VariableEntity("x")));
    relationships.push_back(
        new FollowsRelationship(new AssignStmtEntity(stmt_a), 
                                new ReadStmtEntity(stmt_b)));
    expected_modifies.insert(new VariableEntity("x"));
    expected_follows.insert(new ReadStmtEntity(stmt_b)); 
    expected_modifies_inverse.insert(new AssignStmtEntity(stmt_a));
    expected_follows_inverse.insert(new AssignStmtEntity(stmt_a));
  }

  auto *relationship_manager = new RelationshipManager();
  relationship_manager->Populate(relationships);
  auto *modifies_table = relationship_manager->GetTable(RsType::kModifies);
  auto *follows_table = relationship_manager->GetTable(RsType::kFollows);

  int modifies_count = 0;
  int inverse_modifies_count = 0;
  int modifies_key_count = 0;
  int inverse_modifies_key_count = 0;
  int follows_count = 0;
  int inverse_follows_count = 0;
  int follows_key_count = 0;
  int inverse_follows_key_count = 0;
  EntityPointerUnorderedMap modifies_map = modifies_table->GetTable();
  EntityPointerUnorderedMap inverse_modifies_map = modifies_table->GetTable(true);
  EntityPointerUnorderedMap follows_map = follows_table->GetTable();
  EntityPointerUnorderedMap inverse_follows_map = follows_table->GetTable(true);

  for (auto &key : modifies_map) {
    modifies_count += key.second.size();
    actual_modifies.insert(key.second.begin(), key.second.end());   
    modifies_key_count++;
  }
  for (auto &inverse_key : inverse_modifies_map) {
    inverse_modifies_count += inverse_key.second.size();
    actual_modifies_inverse.insert(inverse_key.second.begin(), inverse_key.second.end());    
    inverse_modifies_key_count++;
  }
  for (auto &key : follows_map) {
    follows_count += key.second.size();
    actual_follows.insert(key.second.begin(), key.second.end());    
    follows_key_count++;
  }
  for (auto &inverse_key : inverse_follows_map) {
    inverse_follows_count += inverse_key.second.size();
    actual_follows_inverse.insert(inverse_key.second.begin(), inverse_key.second.end());  
    inverse_follows_key_count++;
  }

  bool compare_modifies = PKBTestHelper::set_compare(expected_modifies, actual_modifies);
  bool compare_modifies_inverse = PKBTestHelper::set_compare(expected_modifies_inverse, actual_modifies_inverse);
  bool compare_follows = PKBTestHelper::set_compare(expected_follows, actual_follows);
  bool compare_follows_inverse = PKBTestHelper::set_compare(expected_follows_inverse, actual_follows_inverse);

  ASSERT_EQ(modifies_count, 500);
  ASSERT_EQ(modifies_key_count, 500);
  ASSERT_EQ(inverse_modifies_count, 500);
  ASSERT_EQ(inverse_modifies_key_count, 1);
  ASSERT_EQ(compare_modifies, true);
  ASSERT_EQ(compare_modifies_inverse, true);

  ASSERT_EQ(follows_count, 500);
  ASSERT_EQ(follows_key_count, 500);
  ASSERT_EQ(inverse_follows_count, 500);
  ASSERT_EQ(inverse_follows_key_count, 500);
  ASSERT_EQ(compare_follows, true);
  ASSERT_EQ(compare_follows_inverse, true);
}
