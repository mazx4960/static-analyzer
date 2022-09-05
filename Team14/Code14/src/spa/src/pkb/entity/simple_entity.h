// Copyright 2022 CS3203 Team14. All rights reserved.

#pragma once

#include <string>
#include <unordered_set>

#include "pkb/entity/result.h"

class SimpleEntity {
 public:
  virtual std::unordered_set<std::string> *populate(std::string) = 0;
  virtual Result getResult(QuerySynonym &) = 0;
};
