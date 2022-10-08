// Copyright 2022 CS3203 Team14. All rights reserved.

#pragma once

#include <unordered_map>
#include <unordered_set>

#include "commons/entity.h"
#include "commons/pattern.h"
using EntityStringPair = std::pair<Entity *, std::string>;
using EntityStringPairUnorderedSet = std::unordered_set<EntityStringPair, PairHash>;
using PatternEntityUnorderedMap =
    std::unordered_map<Entity *, EntityStringPairUnorderedSet, EntityHashFunction, EntityPointerEquality>;

class PatternTable {
 private:
  std::unordered_map<Entity *, std::unordered_set<std::pair<Entity *, std::string>, PairHash>, EntityHashFunction,
                     EntityPointerEquality>
      pattern_table_;// stores a mapping for the variable to a set of entity and expression pairs
 public:
  PatternTable();
  void Populate(Pattern &pattern);
  PatternEntityUnorderedMap GetTable();
  EntityStringPairUnorderedSet Get(Entity *variable);
};
