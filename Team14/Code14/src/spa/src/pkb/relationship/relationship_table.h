// Copyright 2022 CS3203 Team14. All rights reserved.

#pragma once

#include <string>
#include <unordered_set>
#include <unordered_map>

#include "commons/entity.h"
#include "commons/relationship.h"
#include "commons/result.h"
#include "commons/types.h"

class RelationshipTable {
 protected:
  std::unordered_map<Entity *, 
	  std::unordered_set<Entity *, EntityHashFunction, EntityPointerEquality>, 
	  EntityHashFunction, EntityPointerEquality> table_;

  std::unordered_map<Entity *, 
	  std::unordered_set<Entity *, EntityHashFunction, EntityPointerEquality>,
	  EntityHashFunction, EntityPointerEquality> inverse_table_;

  RelationshipTable() = default;

 public: 
  void populate(Relationship &relationship);   

  // Currently only used for debugging and testing
  int getCount();
};
 
class UsesTable : public RelationshipTable {
 public:
  UsesTable() = default;
};

class UsesInverseTable : public RelationshipTable {
 public:
  UsesInverseTable() = default;
};

class ModifiesTable : public RelationshipTable {
 public:
  ModifiesTable() = default;
};

class ModifiesInverseTable : public RelationshipTable {
 public:
  ModifiesInverseTable() = default;
};

class FollowsTable : public RelationshipTable {
 public:
  FollowsTable() = default;
};

class FollowsInverseTable : public RelationshipTable {
 public:
  FollowsInverseTable() = default;
};
	
	
class ParentTable : public RelationshipTable {
 public:
  ParentTable() = default;
};

class ParentInverseTable : public RelationshipTable {
 public:
  ParentInverseTable() = default;
};
