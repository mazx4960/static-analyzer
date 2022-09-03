// Copyright 2022 CS3203 Team14. All rights reserved.

#include "pkb/entity/variable.h"
#include "pkb/entity/simple_entity.h"
#include <string>
#include <list> 

std::map<std::string, std::list<int>>* Variable::populate(std::string variable, std::list<int> lines) {
  //std::map<std::string, std::list<int>> variableTable; 
  variableTable.insert(std::pair<std::string, std::list<int>>(variable, lines));
  return &variableTable;
};