// Copyright 2022 CS3203 Team14. All rights reserved.

#pragma once

#include <stack>
#include <unordered_map>
#include <unordered_set>

#include "commons/entity.h"
#include "commons/relationship.h"
#include "commons/types.h"
#include "pkb/cache.h"
#include "pkb/exception.h"
#include "relationship_table.h"

using EntityRsInv = std::tuple<Entity *, RsType, bool>;

class RelationshipManager {
 private:
  Cache<EntityRsInv, EntityPointerUnorderedSet, TripletHash> *cache_;
  std::unordered_map<RsType, RelationshipTable *> relationship_table_map_;

  bool IsVariableModified(EntityPointerUnorderedSet *, Entity *);
  bool IsVariableUsed(EntityPointerUnorderedSet *, Entity *);
  Entity *GetProcedureEntity(Entity *, bool);
  EntityPointerUnorderedSet GetAffects(Entity *, bool);
  EntityPointerUnorderedSet GetAffectsHelper(std::stack<std::pair<Entity *, EntityPointerUnorderedSet>> *);
  EntityPointerUnorderedSet GetAffectsInverseHelper(std::stack<std::pair<Entity *, EntityPointerUnorderedSet>> *);
  EntityPointerUnorderedSet GetCalls(Entity *, bool);
  EntityPointerUnorderedSet GetInference(RsType, Entity *, bool);
  EntityPointerUnorderedSet GetInferenceFromChildren(RelationshipTable *, Entity *);
  EntityPointerUnorderedSet GetInferenceGivenProcedure(RelationshipTable *, Entity *);
  EntityPointerUnorderedSet GetInferenceGivenVariable(RelationshipTable *, Entity *);
  EntityPointerUnorderedSet GetSubMatches(RsType, Entity *, bool);
  void CreateTable(RsType);

  static EntityPointerUnorderedSet Empty();
  static EntityRsInv GetCacheQuery(Entity *entity, RsType rs_type, bool is_inverse);

 public:
  RelationshipManager();
  void ClearCache();
  RelationshipTable *GetTable(RsType rs_type);
  void Populate(std::vector<Relationship *> &relationships);
  EntityPointerUnorderedSet Get(RsType, Entity *, bool);
  EntityPointerUnorderedSet GetAll(RsType, Entity *, bool);
};
