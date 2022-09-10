
#pragma once

#include <unordered_set>
#include <unordered_map>

#include "commons/entity.h"
#include "commons/relationship.h"
#include "commons/result.h"

class RelationshipTable {
 protected:
  std::unordered_set<std::pair<Entity*, Entity*>> table_;

  RelationshipTable() = default;

 public:
  static RelationshipTable *getTable(RsType);
  Result get(RsType, Entity*, Entity*);
  Result getStatements(QuerySynonym &, Entity*, Entity*);
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