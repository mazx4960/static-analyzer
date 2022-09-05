// Copyright 2022 CS3203 Team14. All rights reserved.

#include "pkb/entity/variable.h"

#include <unordered_set>
#include <map>
#include <string>

#include "pkb/entity/simple_entity.h"

std::unordered_set<std::string> *Variable::populate(std::string variable) {
  variable_table_.insert(variable);
  return &variable_table_;
}

Result Variable::getResult(QuerySynonym &syn) {
  return Result(syn, variable_table_);
}