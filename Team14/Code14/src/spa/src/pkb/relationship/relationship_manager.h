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
  bool IsResultInCache(EntityRsInv *);
  Entity *GetProcedureEntity(Entity *, bool);
  EntityPointerUnorderedSet GetAffects(Entity *, bool);
  EntityPointerUnorderedSet GetAffectsHelper(std::stack<std::pair<Entity *, EntityPointerUnorderedSet>> *);
  EntityPointerUnorderedSet GetAffectsInverseHelper(std::stack<std::pair<Entity *, EntityPointerUnorderedSet>> *);
  EntityPointerUnorderedSet GetAll(RsType, Entity *, bool);
  EntityPointerUnorderedSet GetCalls(Entity *, bool);
  EntityPointerUnorderedSet GetInference(RsType, Entity *, bool);
  EntityPointerUnorderedSet GetInferenceFromChildren(RsType, Entity *);
  EntityPointerUnorderedSet GetInferenceGivenCallStatement(RsType, Entity *);
  EntityPointerUnorderedSet GetInferenceGivenVariable(RsType, Entity *);
  EntityPointerUnorderedSet GetSubMatches(RsType, Entity *, bool);
  EntityPointerUnorderedSet GetResultInCache(EntityRsInv *);
  void CreateTable(RsType);
  void AddResultToCache(EntityRsInv *, EntityPointerUnorderedSet *);

  static EntityPointerUnorderedSet Empty();
  static EntityRsInv GetCacheQuery(Entity *entity, RsType rs_type, bool is_inverse);
  static RsType GetRsTypeMapping(RsType);

 public:
  RelationshipManager();
  ~RelationshipManager();
  EntityPointerUnorderedSet Get(RsType, Entity *, bool);
  RelationshipTable *GetTable(RsType rs_type);
  void ClearCache();
  void Populate(std::vector<Relationship *> &relationships);
  Cache<EntityRsInv, EntityPointerUnorderedSet, TripletHash> *GetCache(); //Used for testing
};
