// Copyright 2022 CS3203 Team14. All rights reserved.
#pragma once

#include <string>
#include <unordered_map>
#include <unordered_set>

#include "commons/relationship.h"
#include "commons/result.h"
#include "pkb/entity/entity_manager.h"
#include "pkb/entity/entity_table.h"
#include "pkb/relationship/relationship_table.h"
#include "pkb/pattern/pattern_manager.h"

/*
 * Interface for SP -> PKB
 */
class IPKBPopulator {
 protected:
  IPKBPopulator() = default;

 public:
  ~IPKBPopulator() = default;
  virtual void populate(std::vector<Entity *>) = 0;
  virtual void populate(std::vector<Pattern *>) = 0;
};

/*
 * Interface for QPS -> PKB
 */
class IPKBQuerier {
 protected:
  IPKBQuerier() = default;

 public:
  ~IPKBQuerier() = default;
  virtual std::unordered_set<Entity *, EntityHashFunction, EntityPointerEquality> getEntities(EntityType) = 0;
  virtual std::unordered_set<Entity *, EntityHashFunction, EntityPointerEquality> getByRelationship(RsType, Entity *,
                                                                                                    bool) = 0;
  virtual std::unordered_set<Entity *, EntityHashFunction, EntityPointerEquality> getByPattern(Entity *,
                                                                                               std::string &) = 0;
};

class PKB : public IPKBPopulator, public IPKBQuerier {
 private:
  EntityManager *entity_manager_;

  std::unordered_map<RsType, RelationshipTable *> relationship_map_;
  static std::unordered_set<Entity *, EntityHashFunction, EntityPointerEquality> Empty();

  PatternManager *pattern_manager_;

 public:
  PKB();

  std::unordered_set<Entity *, EntityHashFunction, EntityPointerEquality> getEntities(EntityType) override;
  std::unordered_set<Entity *, EntityHashFunction, EntityPointerEquality> getByRelationship(RsType, Entity *,
                                                                                            bool) override;
  std::unordered_set<Entity *, EntityHashFunction, EntityPointerEquality> getByPattern(Entity *,
                                                                                       std::string &) override;

  void populate(std::vector<Entity *> entities) override;
  void populate(std::vector<Pattern *> patterns) override;
};
