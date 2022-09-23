// Copyright 2022 CS3203 Team14. All rights reserved.

#pragma once

#include <string>
#include <unordered_set>
#include <unordered_map>
#include <queue>

#include "commons/types.h"
#include "commons/entity.h"
#include "commons/relationship.h"

using EntityPointerUnorderedSet = std::unordered_set<Entity *, EntityHashFunction, EntityPointerEquality>;
using EntityPointerUnorderedMap = std::unordered_map<Entity *,
                                                     EntityPointerUnorderedSet,
                                                     EntityHashFunction,
                                                     EntityPointerEquality>;

class RelationshipTable {
 protected:
  EntityPointerUnorderedMap table_;

  EntityPointerUnorderedMap inverse_table_;

  RelationshipTable();

 private:
  static EntityPointerUnorderedSet Empty();

 public:
  EntityPointerUnorderedMap GetTable(bool inverse = false);
  void populate(Relationship &);
  EntityPointerUnorderedSet get(Entity *, bool);
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

class CallsTable : public RelationshipTable {
 public:
  CallsTable() = default;
};

class NextTable : public RelationshipTable {
 public:
     NextTable() = default;
};
