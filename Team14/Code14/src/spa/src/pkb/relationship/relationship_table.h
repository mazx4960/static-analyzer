
#pragma once

#include <unordered_set>
#include <unordered_map>

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
  static std::unordered_set<Entity*, EntityHashFunction, EntityPointerEquality> Empty();
  std::unordered_set<Entity *, EntityHashFunction, EntityPointerEquality> getTraversal(Entity *, bool);
  std::unordered_set<Entity *, EntityHashFunction, EntityPointerEquality> get(Entity *, bool);
  std::unordered_set<Entity *, EntityHashFunction, EntityPointerEquality>
  traversalHelper(Entity *,
                  std::unordered_map<Entity *, std::unordered_set<Entity *, EntityHashFunction, EntityPointerEquality>,
                      EntityHashFunction, EntityPointerEquality> *);
 public:
  static RelationshipTable *getTable(RsType);
  std::unordered_set<Entity*, EntityHashFunction, EntityPointerEquality> get(RsType, Entity *, bool);

};

class ModifiesTable : public RelationshipTable {
 public:
  ModifiesTable() = default;
};

class UsesTable : public RelationshipTable {
 public:
  UsesTable() = default;
};

class FollowsTable : public RelationshipTable {
 public:
  FollowsTable() = default;
};

class ParentTable : public RelationshipTable {
 public:
  ParentTable() = default;
};