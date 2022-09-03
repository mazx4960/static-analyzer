// Copyright 2022 CS3203 Team14. All rights reserved.

#pragma once

#include <iostream>
#include <map>
#include <string>
#include <list>
#include "pkb/entity/simple_entity.h"

class Variable : public SIMPLEEntity { 
 private:
  std::map<std::string, std::list<int>> variableTable;

 public:
  std::map<std::string, std::list<int>>* populate(std::string variable, std::list<int> lines);
};