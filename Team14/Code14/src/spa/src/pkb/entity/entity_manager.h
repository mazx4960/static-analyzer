// Copyright 2022 CS3203 Team14. All rights reserved.

#pragma once
#include "pkb/entity/variable.h"

class EntityManager {
 private:
  Variable v;

 public:
  std::map<std::string, std::list<int>>* populate(std::string variable, std::list<int> lines);
};