
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
  std::unordered_set<Entity *> getMatchingEntity(Entity *, bool);
  std::unordered_set<Entity *> getMatchingFirstEntity(Entity *);
  std::unordered_set<Entity *> getMatchingSecondEntity(Entity *);

  std::unordered_set<Entity *> getMatchingStatement(Entity *, bool);
  std::unordered_set<Entity *> getMatchingFirstStatement(Entity *);
  std::unordered_set<Entity *> getMatchingSecondStatement(Entity *);

  std::unordered_set<Entity *> getTraversalStatement(Entity *, bool);

 public:
  static RelationshipTable *getTable(RsType);
  std::unordered_set<Entity*> get(RsType, Entity*, bool);

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