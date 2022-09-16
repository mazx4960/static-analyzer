// Copyright 2022 CS3203 Team14. All rights reserved.

#pragma once

#include <string>
#include <unordered_set>
#include <unordered_map>
#include <queue>

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

  RelationshipTable();

 private:
  static std::unordered_set<Entity *, EntityHashFunction, EntityPointerEquality> Empty();

 public:
  std::unordered_map<Entity *,
                     std::unordered_set<Entity *, EntityHashFunction, EntityPointerEquality>,
                     EntityHashFunction, EntityPointerEquality> GetTable(bool inverse = false);
  void populate(Relationship &);
  std::unordered_set<Entity *, EntityHashFunction, EntityPointerEquality> get(Entity *, bool);
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
