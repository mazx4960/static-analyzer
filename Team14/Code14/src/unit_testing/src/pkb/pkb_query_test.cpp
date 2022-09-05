// Copyright 2022 CS3203 Team14. All rights reserved.

#include "pkb/entity/pkb_query.h"
#include "pkb/entity/result.h"
#include "pkb/entity/variable.h"
#include "pkb/pkb_populator/pkb_populator.h"
#include "pkb/entity/entity_manager.h"

#include "gtest/gtest.h"

TEST(VarQueryTest, EmptyResult) {
		QuerySynonym placeholder = QuerySynonym("placeholder");
		EntityManager manager;
		Result result = manager.getResult(EntityType::kConstant, placeholder);
		
		std::unordered_set<std::string> vResult = result.get_results();
		std::unordered_set<std::string> empty = Result::empty(placeholder).get_results();
		
		ASSERT_EQ(vResult, empty);
}

TEST(VarQueryTest, SameResult) {
		PKBPopulator p;
		QuerySynonym placeholder = QuerySynonym("placeholder");
  Entity x(EntityType::kVariable, "x");
  Entity y(EntityType::kVariable, "y");
		Entity z(EntityType::kVariable, "z"); 
  std::vector<Entity> entities({x, y, z});
  PKBPopulator::populate(entities);

		EntityManager manager;

		std::unordered_set<std::string> vResult = manager.getResult(EntityType::kVariable, placeholder).get_results();
		std::unordered_set<std::string> expected = {"x", "y", "z"};

		ASSERT_EQ(vResult, expected);
}
