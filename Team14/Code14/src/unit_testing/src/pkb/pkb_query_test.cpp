// Copyright 2022 CS3203 Team14. All rights reserved.

#include "commons/pkb_query.h"

#include "commons/result.h"
#include "gtest/gtest.h"
#include "pkb/pkb.h"

TEST(VarQueryTest, EmptyResult) {
  PKB pkb;
  QuerySynonym placeholder = QuerySynonym("placeholder");
  Result result = pkb.getResults(EntityType::kConstant, placeholder);

  std::unordered_set<std::string> v_result = result.get_results_set();
  std::unordered_set<std::string> empty_result = Result::empty(placeholder).get_results_set();

  ASSERT_EQ(v_result, empty_result);
}
