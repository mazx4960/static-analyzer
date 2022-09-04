// Copyright 2022 CS3203 Team14. All rights reserved.

#pragma once

#include <iostream>
#include <vector>
#include <map>
#include <string>

#include "pkb/entity/simple_entity.h"

class Variable : public SimpleEntity {
 private:
  std::vector<std::string> variableTable_;

 public:
  std::vector<std::string>* populate(std::string variable) override;
};
