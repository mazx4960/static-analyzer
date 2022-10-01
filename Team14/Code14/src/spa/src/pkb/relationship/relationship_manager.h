// Copyright 2022 CS3203 Team14. All rights reserved.

#pragma once

#include <unordered_map>
#include <unordered_set>

#include "commons/entity.h"
#include "commons/relationship.h"
#include "commons/types.h"
#include "pkb/exception.h"
#include "relationship_table.h"

class RelationshipManager {
 private:
  std::unordered_map<RsType, RelationshipTable *> relationship_table_map_;

  void CreateTable(RsType);
  EntityPointerUnorderedSet GetInference(RsType, Entity *, bool);
  EntityPointerUnorderedSet getInferenceFromProcedure(RelationshipTable *, Entity *);
  EntityPointerUnorderedSet getInferenceFromChildren(RelationshipTable *, Entity *);
  static EntityPointerUnorderedSet Empty();

 public:
  RelationshipManager() = default;
  RelationshipTable *GetTable(RsType rs_type);
  void Populate(std::vector<Relationship *> &relationships);
  EntityPointerUnorderedSet Get(RsType, Entity *, bool);
  EntityPointerUnorderedSet GetAll(RsType, Entity *, bool);
};
