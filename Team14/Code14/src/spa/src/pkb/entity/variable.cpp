// Copyright 2022 CS3203 Team14. All rights reserved.

#include "pkb/entity/variable.h"

#include <unordered_set>
#include <map>
#include <string>

#include "pkb/entity/simple_entity.h"

std::unordered_set<std::string> Variable::variableTable_;

Variable::Variable() {
  
}

std::unordered_set<std::string> *Variable::populate(std::string variable) {
  variableTable_.insert(variable);
  return &variableTable_;
} 

Result Variable::getResult(QuerySynonym &syn_) {
  return Result(syn_, variableTable_);
}