// Copyright 2022 CS3203 Team14. All rights reserved.

#pragma once

#include <vector>
#include <map>
#include <string>

class SimpleEntity {
 public:
  virtual std::vector<std::string>*
  populate(std::string entity) = 0;
};
