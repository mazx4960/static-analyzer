
#pragma once

#include <unordered_set>
#include <unordered_map>

#include "commons/entity.h"
#include "commons/relationship.h"

class RelationshipTable {
 protected:
  std::unordered_map<Entity, std::unordered_set<Entity, EntityHashFunction>, EntityHashFunction> table_;

  RelationshipTable() = default;

 private:
  static std::unordered_set<Entity*> Empty();
  std::unordered_set<Entity *> getStatements(Entity *, bool);
  std::unordered_set<Entity *> getTraversal(Entity *, bool);
  std::unordered_set<Entity *> get(Entity *, bool);
  std::unordered_set<Entity *> traversalHelper(Entity *);
  std::unordered_set<Entity *> formatResults(const std::unordered_set<Entity, EntityHashFunction>&);
 public:
  static RelationshipTable *getTable(RsType);
  std::unordered_set<Entity*> get(RsType, Entity *, bool);
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