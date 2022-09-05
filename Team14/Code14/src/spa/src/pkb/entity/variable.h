// Copyright 2022 CS3203 Team14. All rights reserved.

#pragma once

#include <iostream>
#include <unordered_set>
#include <map>
#include <string>

#include "pkb/entity/simple_entity.h"

class Variable : public SimpleEntity {
 private:
  std::unordered_set<std::string> variable_table_;

 public:
  Variable() = default;
  void populate(std::string) override;
  Result getResult(QuerySynonym &) override;
};
