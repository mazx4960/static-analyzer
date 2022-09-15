// Copyright 2022 CS3203 Team14. All rights reserved.

#pragma once

#include <string>
#include <unordered_set>
#include <unordered_map>

#include "commons/types.h"
#include "commons/entity.h"
#include "commons/relationship.h"

class RelationshipTable {
 protected:
  std::unordered_map<Entity *,
                     std::unordered_set<Entity *, EntityHashFunction, EntityPointerEquality>,
                     EntityHashFunction, EntityPointerEquality> table_;

  std::unordered_map<Entity *,
                     std::unordered_set<Entity *, EntityHashFunction, EntityPointerEquality>,
                     EntityHashFunction, EntityPointerEquality> inverse_table_;

  RelationshipTable() = default;

 private:
  static std::unordered_set<Entity *, EntityHashFunction, EntityPointerEquality> Empty();
  std::unordered_set<Entity *, EntityHashFunction, EntityPointerEquality> getTraversal(Entity *, bool);
  std::unordered_set<Entity *, EntityHashFunction, EntityPointerEquality> get(Entity *, bool);
  std::unordered_map<Entity *,
                     std::unordered_set<Entity *, EntityHashFunction, EntityPointerEquality>,
                     EntityHashFunction,
                     EntityPointerEquality> get_inverse();
  std::unordered_set<Entity *,
                     EntityHashFunction,
                     EntityPointerEquality> traversalHelper(Entity *,
                                                            std::unordered_map<Entity *,
                                                                               std::unordered_set<
                                                                                   Entity *,
                                                                                   EntityHashFunction,
                                                                                   EntityPointerEquality>,
                                                                               EntityHashFunction,
                                                                               EntityPointerEquality> *);
 public:
  std::unordered_map<Entity *,
                     std::unordered_set<Entity *, EntityHashFunction, EntityPointerEquality>,
                     EntityHashFunction, EntityPointerEquality> GetTable();
  std::unordered_map<Entity *,
                     std::unordered_set<Entity *, EntityHashFunction, EntityPointerEquality>,
                     EntityHashFunction, EntityPointerEquality> GetInverseTable();
  void populate(Relationship &);
  std::unordered_set<Entity *, EntityHashFunction, EntityPointerEquality> get(RsType, Entity *, bool);
};

class UsesTable : public RelationshipTable {
 public:
  UsesTable() = default;
};

class ModifiesTable : public RelationshipTable {
 public:
  ModifiesTable() = default;
};

class FollowsTable : public RelationshipTable {
 public:
  FollowsTable() = default;
};

class ParentTable : public RelationshipTable {
 public:
  ParentTable() = default;
};
