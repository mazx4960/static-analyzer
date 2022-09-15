// Copyright 2022 CS3203 Team14. All rights reserved.

#pragma once

#include <unordered_map>
#include <unordered_set>

#include "commons/entity.h"
#include "commons/pattern.h"

class PatternTable {
 private:
  std::unordered_map<Entity *, std::unordered_set<std::pair<Entity *, std::string>, StmtExprHashFunction>,
                     EntityHashFunction,
                     EntityPointerEquality>
      pattern_table_;// stores a mapping for the variable to a set of entity and expression pairs
 public:
  PatternTable();
  void Populate(Pattern &pattern);
  std::unordered_map<Entity *, std::unordered_set<std::pair<Entity *, std::string>, StmtExprHashFunction>,
                     EntityHashFunction, EntityPointerEquality>
  GetTable();
  std::unordered_set<Entity *> Get(Entity &variable, std::string &expression);
};
