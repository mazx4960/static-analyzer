// Copyright 2022 CS3203 Team14. All rights reserved.

#pragma once

#include <unordered_map>
#include <unordered_set>

#include "commons/entity.h"
#include "commons/types.h"
#include "commons/relationship.h"
#include "relationship_table.h"
#include "pkb/exception.h"

class RelationshipManager {
 private:
  std::unordered_map<RsType, RelationshipTable *> relationship_table_map_;
  std::unordered_set<Entity *,
                     EntityHashFunction,
                     EntityPointerEquality> getTraversal(RsType, Entity *, bool);
  static std::unordered_set<Entity *,
                     EntityHashFunction,
                     EntityPointerEquality> traversalHelper(Entity *,
                                                            std::unordered_map<Entity *,
                                                               std::unordered_set<
                                                                  Entity *,
                                                                  EntityHashFunction,
                                                                  EntityPointerEquality>,
                                                           EntityHashFunction,
                                                           EntityPointerEquality> *);
  void CreateTable(RsType);

 public:
  RelationshipManager() = default;
  RelationshipTable *GetTable(RsType rs_type);
  void Populate(std::vector<Relationship *> &relationships);
  std::unordered_set<Entity *, EntityHashFunction, EntityPointerEquality> Get(RsType, Entity *, bool);
};
