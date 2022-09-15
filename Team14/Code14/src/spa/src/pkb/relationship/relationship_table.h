
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

  RelationshipTable() = default;

 private:
  static std::unordered_set<Entity*, EntityHashFunction, EntityPointerEquality> Empty();
  std::unordered_set<Entity *, EntityHashFunction, EntityPointerEquality> getStatements(Entity *, bool);
  std::unordered_set<Entity *, EntityHashFunction, EntityPointerEquality> getTraversal(Entity *, bool);
  std::unordered_set<Entity *, EntityHashFunction, EntityPointerEquality> get(Entity *, bool);
  std::unordered_set<Entity *, EntityHashFunction, EntityPointerEquality> traversalHelper(Entity *);
 public:
  static RelationshipTable *getTable(RsType);
  std::unordered_set<Entity*, EntityHashFunction, EntityPointerEquality> get(RsType, Entity *, bool);
  void populate(Relationship &);
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