// Copyright 2022 CS3203 Team14. All rights reserved.

#include "pattern_table.h"

PatternTable::PatternTable() {
  this->pattern_table_ = PatternEntityUnorderedMap();
}

void PatternTable::Populate(Pattern &pattern) {
  Entity *variable = pattern.GetVariable();
  Entity *stmt = pattern.GetStmt();
  std::string expression = pattern.GetExpr();
  std::pair<Entity *, std::string> pair = std::pair<Entity *, std::string>(stmt, expression);
  if (this->pattern_table_.find(variable) == this->pattern_table_.end()) {
    EntityStringPairUnorderedSet set = EntityStringPairUnorderedSet();
    set.insert(pair);
    this->pattern_table_[variable] = set;
  } else {
    this->pattern_table_[variable].insert(pair);
  }
}

EntityStringPairUnorderedSet PatternTable::Get(Entity *variable) {
  return this->pattern_table_[variable];
}

PatternEntityUnorderedMap PatternTable::GetTable() {
  return this->pattern_table_;
}
