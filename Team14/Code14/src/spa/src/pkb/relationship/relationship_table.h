
#pragma once

#include <unordered_set>
#include <unordered_map>

#include "commons/entity.h"
#include "commons/relationship.h"

class RelationshipTable {
 protected:
  std::unordered_set<std::pair<Entity*, Entity*>, RelationshipTableHashingFunction> table_;

  RelationshipTable() = default;

 private:
  std::unordered_set<Entity *> getMatchingEntity(Entity *, Entity *);
  std::unordered_set<Entity *> getMatchingStatement(Entity *, Entity *);
  std::unordered_set<Entity *> getTraversalStatement(Entity *, Entity *);

 public:
  static RelationshipTable *getTable(RsType);
  std::unordered_set<Entity*> get(RsType, Entity *, Entity *);

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