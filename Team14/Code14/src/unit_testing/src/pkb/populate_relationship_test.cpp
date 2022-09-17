// Copyright 2022 CS3203 Team14. All rights reserved.

#include <gtest/gtest.h>

#include <vector>

#include "commons/entity.h"
#include "commons/relationship.h"
#include "pkb/pkb.h"

TEST(PopulateRelationshipTest, SingleKeyTest) {
  std::vector<std::pair<std::string, std::string>> modifies = {{"1", "w"}, {"1", "x"}, {"1", "y"}, {"1", "z"}};
  std::vector<Relationship *> relationships;
  int length = modifies.size();
  relationships.reserve(length);

  for (int i = 0; i < length; i++) {
    relationships.push_back(
        new ModifiesRelationship(new AssignStmtEntity(modifies[i].first), new VariableEntity(modifies[i].second)));
  }

  auto *relationship_manager = new RelationshipManager();
  relationship_manager->Populate(relationships);
  auto *relationship_table = relationship_manager->GetTable(RsType::kModifies);
   
  int key_count = 0; 
  int inverse_key_count = 0;

  for (auto &key : relationship_table->GetTable()) { 
    key_count++;
  }

  for (auto &inverse_key : relationship_table->GetTable(true)) { 
    inverse_key_count++;
  }

  ASSERT_EQ(key_count, 1);
  ASSERT_EQ(inverse_key_count, 4);
}

TEST(PopulateRelationshipTest, MultipleKeysTest) {
  std::vector<std::pair<std::string, std::string>> uses = {{"1", "w"}, {"2", "x"}, {"3", "y"}, {"4", "z"}};
  std::vector<Relationship *> relationships;
  int length = uses.size();
  relationships.reserve(length);

  for (int i = 0; i < length; i++) {
    relationships.push_back(
        new ModifiesRelationship(new AssignStmtEntity(uses[i].first), new VariableEntity(uses[i].second)));
  }

  auto *relationship_manager = new RelationshipManager();
  relationship_manager->Populate(relationships);
  auto *relationship_table = relationship_manager->GetTable(RsType::kModifies);

  int count = 0;
  int key_count = 0;
  int inverse_count = 0;
  int inverse_key_count = 0;

  for (auto &key : relationship_table->GetTable()) {
    count += key.second.size();
    key_count++;
  }

  for (auto &inverse_key : relationship_table->GetTable(true)) {
    inverse_count += inverse_key.second.size();
    inverse_key_count++;
  }

  ASSERT_EQ(key_count, 4);
  ASSERT_EQ(inverse_key_count, 4);
}

TEST(PopulateRelationshipTest, FollowsTest) {
  std::vector<std::pair<std::string, std::string>> follows = {{"1", "2"}, {"2", "3"}, {"3", "4"}, {"4", "5"}};
  std::vector<Relationship *> relationships;
  int length = follows.size();
  relationships.reserve(length);

  for (int i = 0; i < length; i++) {
    relationships.push_back(
        new FollowsRelationship(new AssignStmtEntity(follows[i].first), new AssignStmtEntity(follows[i].second)));
  }

  auto *relationship_manager = new RelationshipManager();
  relationship_manager->Populate(relationships);
  auto *relationship_table = relationship_manager->GetTable(RsType::kFollows);

  int count = 0;
  int inverse_count = 0; 

  for (auto &key : relationship_table->GetTable()) {
    count += key.second.size(); 
  }

  for (auto &inverse_key : relationship_table->GetTable(true)) {
    inverse_count += inverse_key.second.size(); 
  }

  ASSERT_EQ(count, length);
  ASSERT_EQ(inverse_count, length);
} 

TEST(PopulateRelationshipTest, UsesSingleStmtEntityTest) {
  std::vector<std::pair<std::string, std::string>> uses = {{"1", "x"}, {"2", "y"}, {"3", "z"}, {"4", "w"}};
  std::vector<Relationship *> relationships;
  int length = uses.size();
  relationships.reserve(length);

  for (int i = 0; i < length; i++) {
    relationships.push_back(
        new UsesRelationship(new AssignStmtEntity(uses[i].first), new VariableEntity(uses[i].second)));
  }

  auto *relationship_manager = new RelationshipManager();
  relationship_manager->Populate(relationships);
  auto *relationship_table = relationship_manager->GetTable(RsType::kUses);

  int count = 0;
  int inverse_count = 0;
  for (auto &key : relationship_table->GetTable()) { count += key.second.size(); }

  for (auto &inverse_key : relationship_table->GetTable(true)) { inverse_count += inverse_key.second.size(); }

  ASSERT_EQ(count, length);
  ASSERT_EQ(inverse_count, length);
}

TEST(PopulateRelationshipTest, UsesMixedStmtEntityTest) {
  std::vector<std::pair<std::string, std::string>> uses_assign = {{"1", "x"}, {"1", "y"}, {"2", "z"}};
  std::vector<std::pair<std::string, std::string>> uses_print = {{"3", "x"}, {"3", "y"}, {"4", "z"}, {"5", "w"}};
  std::vector<Relationship *> relationships;
  int uses_assign_length = uses_assign.size();
  int uses_print_length = uses_print.size();
  int length = uses_assign_length + uses_print_length;
  relationships.reserve(length);

  for (int i = 0; i < uses_assign_length; i++) {
    relationships.push_back(
        new UsesRelationship(new AssignStmtEntity(uses_assign[i].first), new VariableEntity(uses_assign[i].second)));
  }

  for (int j = 0; j < uses_print_length; j++) {
    relationships.push_back(
        new UsesRelationship(new PrintStmtEntity(uses_print[j].first), new VariableEntity(uses_print[j].second)));
  }

  auto *relationship_manager = new RelationshipManager();
  relationship_manager->Populate(relationships);
  auto *relationship_table = relationship_manager->GetTable(RsType::kUses);

  int count = 0;
  int inverse_count = 0;
  for (auto &key : relationship_table->GetTable()) { count += key.second.size(); }

  for (auto &inverse_key : relationship_table->GetTable(true)) { inverse_count += inverse_key.second.size(); }

  ASSERT_EQ(count, length);
  ASSERT_EQ(inverse_count, length);
}

TEST(PopulateRelationshipTest, MultipleRelationshipsTest) {
  std::vector<std::pair<std::string, std::string>> uses = {{"1", "x"}, {"1", "y"}, {"2", "z"}, {"1", "u"}};
  std::vector<std::pair<std::string, std::string>> modifies = {{"3", "x"}, {"4", "y"}, {"5", "z"}, {"6", "w"}};
  std::vector<Relationship *> relationships;
  int uses_length = uses.size();
  int modifies_length = modifies.size(); 
  relationships.reserve(uses_length + modifies_length);

  for (int i = 0; i < uses_length; i++) {
    relationships.push_back(
        new UsesRelationship(new AssignStmtEntity(uses[i].first), new VariableEntity(uses[i].second)));
  }

  for (int j = 0; j < modifies_length; j++) {
    relationships.push_back(
        new ModifiesRelationship(new ReadStmtEntity(modifies[j].first), new VariableEntity(modifies[j].second)));
  }

  auto *relationship_manager = new RelationshipManager();
  relationship_manager->Populate(relationships);
  auto *uses_table = relationship_manager->GetTable(RsType::kUses);
  auto *modifies_table = relationship_manager->GetTable(RsType::kModifies);

  int uses_count = 0;
  int uses_inverse_count = 0;
  int modifies_count = 0;
  int modifies_inverse_count = 0;

  for (auto &key : uses_table->GetTable()) { uses_count += key.second.size(); }
  for (auto &inverse_key : uses_table->GetTable(true)) { uses_inverse_count += inverse_key.second.size(); }

  for (auto &key : modifies_table->GetTable()) { modifies_count += key.second.size(); }
  for (auto &inverse_key : modifies_table->GetTable(true)) { modifies_inverse_count += inverse_key.second.size(); }

  ASSERT_EQ(uses_count, uses_length);
  ASSERT_EQ(uses_inverse_count, uses_length);
  ASSERT_EQ(modifies_count, modifies_length);
  ASSERT_EQ(modifies_inverse_count, modifies_length);
}

TEST(PopulateRelationshipTest, DuplicateRelationshipsTest) {
  std::vector<std::pair<std::string, std::string>> parent = {{"1", "3"}, {"1", "4"}, {"5", "6"}, {"5", "7"}};
  std::vector<Relationship *> relationships;
  int length = parent.size();
  relationships.reserve(length * 2);

  for (int i = 0; i < length; i++) {
    for (int j = 0; j < 2; j++) {
      relationships.push_back(
          new ParentRelationship(new WhileStmtEntity(parent[i].first), new AssignStmtEntity(parent[i].second)));
    }
  }

  auto *relationship_manager = new RelationshipManager();
  relationship_manager->Populate(relationships);
  auto *relationship_table = relationship_manager->GetTable(RsType::kParent);

  int count = 0;
  int inverse_count = 0;
  for (auto &key : relationship_table->GetTable()) { count += key.second.size(); }

  for (auto &inverse_key : relationship_table->GetTable(true)) { inverse_count += inverse_key.second.size(); }

  ASSERT_EQ(count, length);
  ASSERT_EQ(inverse_count, length);
}