// Copyright 2022 CS3203 Team14. All rights reserved.

#include "pattern_table.h"

PatternTable::PatternTable() {
  this->pattern_map_ =
      std::unordered_map<Entity*, std::unordered_set<std::pair<Entity*, std::string>, StmtExprHashFunction>,
                         EntityHashFunction, EntityPointerEquality>();
}
void PatternTable::Populate(Pattern& pattern) {
  Entity* variable = pattern.GetVariable();
  Entity* stmt = pattern.GetStmt();
  std::string expression = pattern.GetExpr();
  std::pair<Entity*, std::string> pair = std::pair<Entity*, std::string>(stmt, expression);
  if (this->pattern_map_.find(variable) == this->pattern_map_.end()) {
    std::unordered_set<std::pair<Entity*, std::string>, StmtExprHashFunction> set =
        std::unordered_set<std::pair<Entity*, std::string>, StmtExprHashFunction>();
    set.insert(pair);
    this->pattern_map_[variable] = set;
  } else {
    this->pattern_map_[variable].insert(pair);
  }
}
std::unordered_set<Entity*> PatternTable::Get(Entity& variable, std::string& expression) {
  return std::unordered_set<Entity*>();
}
