// Copyright 2022 CS3203 Team14. All rights reserved.

#pragma once

#include <unordered_set>
#include <vector>
#include <map>
#include <string>

class SimpleEntity {
 public:
  virtual std::unordered_set<std::string>*
  populate(std::string entity) = 0;
};
