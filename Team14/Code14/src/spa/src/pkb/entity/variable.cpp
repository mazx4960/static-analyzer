// Copyright 2022 CS3203 Team14. All rights reserved.

#include "variable.h"

#include <unordered_set>
#include "pkb/entity/simple_entity.h"

void VariableTable::populate(std::string variable) {
  this->variable_table_.insert(variable);
  //return &this->variable_table_;
}

Result VariableTable::getResult(QuerySynonym &syn) {
  return Result(syn, this->variable_table_);
}