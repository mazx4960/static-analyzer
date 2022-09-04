// Copyright 2022 CS3203 Team14. All rights reserved.

#include "pkb/entity/variable.h"

#include <vector>
#include <map>
#include <string>

#include "pkb/entity/simple_entity.h"

std::vector<std::string>* Variable::populate(std::string variable) {
  variableTable_.push_back(variable);
  return &variableTable_;
} 