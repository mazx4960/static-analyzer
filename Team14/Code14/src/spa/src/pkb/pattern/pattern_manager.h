// Copyright 2022 CS3203 Team14. All rights reserved.

#pragma once

#include <unordered_set>
#include <vector>

#include "commons/entity.h"
#include "commons/pattern.h"
#include "pattern_table.h"

class PatternManager {
 private:
  PatternTable *pattern_table_;

 public:
  PatternManager();
  PatternTable *GetTable();
  void Populate(const std::vector<Pattern *> &patterns);
  std::unordered_set<Entity *, EntityHashFunction, EntityPointerEquality> Get(Entity *variable,
                                                                              const std::string &expr);
};