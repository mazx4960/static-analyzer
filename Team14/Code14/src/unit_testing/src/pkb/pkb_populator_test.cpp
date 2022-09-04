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

    ptr = p.populate(entities); 

    std::list<std::string> expectedTable{"x", "y"};

    std::list<std::string> resultTable = *ptr;
    std::cout << resultTable.size() << "\n";

    ASSERT_EQ(resultTable.size(), expectedTable.size());
    ASSERT_EQ(resultTable, expectedTable);

    std::list<std::string>::iterator result = resultTable.begin();
    std::list<std::string>::iterator expected = expectedTable.begin();

    for (; result != resultTable.end() && expected != expectedTable.end();
        ++result, ++expected) {
      EXPECT_EQ(*result, *expected) << "Variables " << *result << " and " << *expected << " are not the same."; 
    } 
}
