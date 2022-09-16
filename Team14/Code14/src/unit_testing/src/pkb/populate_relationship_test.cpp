// Copyright 2022 CS3203 Team14. All rights reserved.

#include <gtest/gtest.h>

#include <vector>

#include "commons/entity.h"
#include "commons/relationship.h"
#include "pkb/pkb.h"

TEST(PopulateRelationshipTest, FollowsTest) {
  std::vector<std::pair<std::string, std::string>> follows = {{"1", "2"}, {"1", "3"}, {"1", "4"}, {"1", "6"}};
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
  for (auto &key : relationship_table->GetTable()) { count += key.second.size(); }

  for (auto &inverse_key : relationship_table->GetTable(true)) { inverse_count += inverse_key.second.size(); }

  ASSERT_EQ(count, length);
  ASSERT_EQ(inverse_count, length);
}