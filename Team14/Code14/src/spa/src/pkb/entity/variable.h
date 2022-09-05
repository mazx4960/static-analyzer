// Copyright 2022 CS3203 Team14. All rights reserved.

#pragma once

#include <iostream>
#include <unordered_set>
#include <map>
#include <string>

#include "pkb/entity/simple_entity.h"

class Variable : public SimpleEntity {
 private:
  static std::unordered_set<std::string> variableTable_;

 public:
  std::unordered_set<std::string> *populate(std::string) override;
  Result getResult(QuerySynonym &);
  Variable();
};
