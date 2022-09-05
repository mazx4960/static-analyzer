// Copyright 2022 CS3203 Team14. All rights reserved.

#include "pkb/entity/pkb_query.h"
#include "pkb/entity/result.h"
#include "pkb/entity_manager.h"

#include "gtest/gtest.h"

TEST(VarQueryTest, EmptyResult) {
  QuerySynonym placeholder = QuerySynonym("placeholder");
  EntityManager manager;
  Result result = manager.getResult(EntityType::kConstant, placeholder);

  std::unordered_set<std::string> v_result = result.get_results();
  std::unordered_set<std::string> empty = Result::empty(placeholder).get_results();

  ASSERT_EQ(v_result, empty);
}
