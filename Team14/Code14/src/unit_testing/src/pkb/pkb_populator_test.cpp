// Copyright 2022 CS3203 Team14. All rights reserved.

#include "pkb/pkb_populator/pkb_populator.h"

#include <gtest/gtest.h>

#include <iostream>
#include <list>
#include <map>
#include <string>

#include "commons/types.h"
#include "commons/entity.h"

TEST(PKBPopulatorTest, TestBasic) {
    PKBPopulator p;
    std::list<std::string>* ptr;

    Entity x(EntityType::kVariable, "x");
    Entity y(EntityType::kVariable, "y"); 
    std::list<Entity> entities({x, y});

    ptr = PKBPopulator::populate(entities); 

    std::list<std::string> expected_table{"x", "y"};

    std::list<std::string> result_table = *ptr;
    std::cout << result_table.size() << "\n";

    ASSERT_EQ(result_table.size(), expected_table.size()); 

    auto result = result_table.begin();
    auto expected = expected_table.begin();

    for (; result != result_table.end() && expected != expected_table.end();
        ++result, ++expected) {
      EXPECT_EQ(*result, *expected) << "Variables " << *result << " and " << *expected << " are not the same."; 
    } 
}
