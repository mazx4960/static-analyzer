#include "pkb/pkb_populator/pkb_populator.h"

#include <gtest/gtest.h>

#include <list>
#include <map>
#include <string>

TEST(PKBPopulatorTest, TestBasic){
    PKBPopulator p;
    std::map<std::string, std::list<int>>* ptr;

    ptr = p.populate("x", {1, 2, 3});
    ptr = p.populate("y", {5, 6});
    std::map<std::string, std::list<int>> expectedTable;
    std::pair<std::string, std::list<int>> one("x", {1, 2, 3});
    std::pair<std::string, std::list<int>> two("y", {5, 6});
    expectedTable.insert(one);
    expectedTable.insert(two);
    std::map<std::string, std::list<int>> variableTable = *ptr;
    ASSERT_EQ(variableTable.size(), expectedTable.size());
    ASSERT_EQ(variableTable, expectedTable);

    for (auto result = variableTable.begin(), expected = expectedTable.begin(); result != variableTable.end(); ++result, ++expected) {
      EXPECT_EQ(result->first, expected->first) << "Variable is not the same";
      EXPECT_EQ(result->second, expected->second) << "Statement lines are not the same";
    }
};