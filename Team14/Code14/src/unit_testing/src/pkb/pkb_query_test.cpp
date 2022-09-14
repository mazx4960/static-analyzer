// Copyright 2022 CS3203 Team14. All rights reserved.


#include "commons/result.h"
#include "gtest/gtest.h"
#include "pkb/pkb.h"
#include "pkb/relationship/relationship_table.h"

// TODO(howtoosee, LeeEnEn): add PKB query test
TEST(QueryTest, FollowsRelationship) {
  std::vector<std::pair<std::string, std::string>> follows = {
      {"1", "2"},
      {"2", "3"},
      {"3", "4"},
      {"8", "10"}
  };

  std::vector<Relationship *> relationships;
  for (int i = 0; i < follows.size(); i++) {
    relationships.push_back(
        new FollowsRelationship(
            new AssignStmtEntity(follows[i].first),
            new AssignStmtEntity(follows[i].second))
    );
  }
  PKB pkb;
  pkb.populate(relationships);

  auto result = pkb.getByRelationship(RsType::kFollows, new AssignStmtEntity("8"), false);
}