// Copyright 2022 CS3203 Team14. All rights reserved.

#pragma once

#include <unordered_set>
#include <vector>

#include "commons/entity.h"
#include "commons/pattern.h"
#include "pattern_table.h"
using EntityPointerUnorderedSet = std::unordered_set<Entity *, EntityHashFunction, EntityPointerEquality>;

class PatternManager {
 private:
  PatternTable *pattern_table_;

 public:
  PatternManager();
  PatternTable *GetTable();
  void Populate(const std::vector<Pattern *> &patterns);
  EntityPointerUnorderedSet Get(Entity *variable, const std::string &expr, bool isWildcard);
};