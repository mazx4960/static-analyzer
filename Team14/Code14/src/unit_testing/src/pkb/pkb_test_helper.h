// Copyright 2022 CS3203 Team14. All rights reserved.

#include "commons/pattern.h"

using EntityPointerUnorderedSet = std::unordered_set<Entity *, EntityHashFunction, EntityPointerEquality>;

class PKBTestHelper {
 public:
  static bool set_compare(EntityPointerUnorderedSet const &lhs, EntityPointerUnorderedSet const &rhs) {
    if (lhs.size() != rhs.size()) { return false; }
    return std::all_of(lhs.begin(), lhs.end(), [&rhs](Entity *entity) { return rhs.find(entity) != rhs.end(); });
  }
};
