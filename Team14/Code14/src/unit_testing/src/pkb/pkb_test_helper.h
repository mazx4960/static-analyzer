// Copyright 2022 CS3203 Team14. All rights reserved.

#include <unordered_set>

#include "commons/pattern.h"

using EntityPointerUnorderedSet = std::unordered_set<Entity *, EntityHashFunction, EntityPointerEquality>;
using EntityStringPair = std::pair<Entity *, std::string>;
using EntityStringPairUnorderedSet = std::unordered_set<EntityStringPair, StmtExprHashFunction>;

class PKBTestHelper {
 public:
  static bool set_compare(EntityPointerUnorderedSet const &lhs, EntityPointerUnorderedSet const &rhs) {
    if (lhs.size() != rhs.size()) { return false; }
    return std::all_of(lhs.begin(), lhs.end(), [&rhs](Entity *entity) { return rhs.find(entity) != rhs.end(); });
  }
  static bool set_pair_compare(EntityStringPairUnorderedSet const& lhs, EntityStringPairUnorderedSet const& rhs) {
    if (lhs.size() != rhs.size()) { return false; }
    return std::all_of(lhs.begin(), lhs.end(), [&rhs](EntityStringPair const& pair) { return rhs.find(pair) != rhs.end(); });
  }
};
