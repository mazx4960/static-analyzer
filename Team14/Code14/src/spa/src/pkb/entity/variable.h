// Copyright 2022 CS3203 Team14. All rights reserved.

#pragma once

#include <iostream>
#include <list>
#include <map>
#include <string>

#include "pkb/entity/simple_entity.h"

class Variable : public SIMPLEEntity {
 private:
  std::list<std::string> variableTable;

 public:
  std::list<std::string>* populate(std::string variable);
};
