// Copyright 2022 CS3203 Team14. All rights reserved.

#pragma once

#include <unordered_map>
#include <unordered_set>

#include "commons/entity.h"
#include "commons/relationship.h"
#include "commons/types.h"
#include "pkb/cache.h"
#include "pkb/exception.h"
#include "relationship_table.h"

using entity_rs_inv = std::tuple<Entity *, RsType, bool>;

class RelationshipManager {
 private:
  Cache<entity_rs_inv, EntityPointerUnorderedSet, TripletHash> *cache_;
  std::unordered_map<RsType, RelationshipTable *> relationship_table_map_;

  static entity_rs_inv GetCacheQuery(Entity *entity, RsType rs_type, bool is_inverse);
  void CreateTable(RsType);

  EntityPointerUnorderedSet GetInference(RsType, Entity *, bool);
  EntityPointerUnorderedSet GetInferenceGivenProcedure(RelationshipTable *, Entity *);
  EntityPointerUnorderedSet GetInferenceFromChildren(RelationshipTable *, Entity *);
  EntityPointerUnorderedSet GetInferenceGivenVariable(RelationshipTable *, Entity *);
  EntityPointerUnorderedSet GetCalls(Entity *, bool);
  EntityPointerUnorderedSet GetAllCalls(Entity *, bool);
  Entity *GetProcedureEntity(Entity *, bool);
  static EntityPointerUnorderedSet Empty();

 public:
  RelationshipManager();
  void ClearCache();
  RelationshipTable *GetTable(RsType rs_type);
  void Populate(std::vector<Relationship *> &relationships);
  EntityPointerUnorderedSet Get(RsType, Entity *, bool);
  EntityPointerUnorderedSet GetAll(RsType, Entity *, bool);
};
