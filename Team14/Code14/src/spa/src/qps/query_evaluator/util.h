// Copyright 2022 CS3203 Team14. All rights reserved.

#pragma once

#include "types.h"

/**
 * Intersect the context of two QueryDeclarations.
 * @param first first set of Entity pointers.
 * @param second second set of Entity pointers.
 * @return intersection of sets of Entity pointers.
 */
inline EntitySet intersect(const EntitySet &first, const EntitySet &second) {
  EntitySet result;
  for (auto *entity : first) {
    if (second.find(entity) != second.end()) {
      result.insert(entity);
    }
  }
  return result;
}

/**
 * Copy the elements of a set of Entity pointers that satisfy a predicate.
 * @param candidates
 * @param pred
 * @return
 */
inline EntitySet copy_if(const EntitySet &candidates, const std::function<bool(Entity *)> &pred) {
  EntitySet result;
  for (auto *entity : candidates) {
    if (pred(entity)) {
      result.insert(entity);
    }
  }
  return result;
}
