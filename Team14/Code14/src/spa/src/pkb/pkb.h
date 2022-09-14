// Copyright 2022 CS3203 Team14. All rights reserved.
#pragma once

#include <string>
#include <unordered_map>
#include <unordered_set>

#include "commons/relationship.h"
#include "commons/result.h"
#include "pkb.h"
#include "pkb/entity/entity_table.h"
#include "pkb/relationship/relationship_table.h"

/*
 * Interface for SP -> PKB
 */
class IPKBPopulator {
 protected:
  IPKBPopulator() = default;
 public:
  ~IPKBPopulator() = default;
  virtual void populate(std::vector<Entity *> &) = 0;
};

/*
 * Interface for QPS -> PKB
 */
class IPKBQuerier {
 protected:
  IPKBQuerier() = default;
 public:
  ~IPKBQuerier() = default;
  virtual std::unordered_set<Entity *> getEntities(EntityType) = 0;
  virtual std::unordered_set<Entity *> getByRelationship(RsType, Entity *, bool) = 0;
  virtual std::unordered_set<Entity *> getByPattern(std::string &, std::string &) = 0;

};

class PKB : public IPKBPopulator, public IPKBQuerier {
 private:
  std::unordered_map<EntityType, EntityTable *> entity_map_;
  std::unordered_map<RsType, RelationshipTable *> relationship_map_;

  static std::unordered_set<Entity *> Empty();

 public:
  PKB() = default;

  std::unordered_set<Entity *> getEntities(EntityType) override;
  std::unordered_set<Entity *> getByRelationship(RsType, Entity *, bool) override;
  std::unordered_set<Entity *> getByPattern(std::string &, std::string &) override;

  void populate(std::vector<Entity *> &entities) override;
  // Currently only used for debugging and testing
  int getCount();
};