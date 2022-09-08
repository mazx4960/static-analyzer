// Copyright 2022 CS3203 Team14. All rights reserved.

#include "pkb/entity/pkb_query.h"
#include "pkb/entity/result.h"

#include "gtest/gtest.h"
#include "pkb/pkb.h"

TEST(VarQueryTest, EmptyResult) {
  PKB pkb;
  QuerySynonym placeholder = QuerySynonym("placeholder");
  Result result = pkb.get(EntityType::kConstant, placeholder);

  std::unordered_set<std::string> v_result = result.get_results_set();
  std::unordered_set<std::string> empty_result = Result::empty(placeholder).get_results_set();

  ASSERT_EQ(v_result, empty_result);
}
