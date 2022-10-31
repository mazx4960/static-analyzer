// Copyright 2022 CS3203 Team14. All rights reserved.
#pragma once

#include <string>
#include <unordered_map>
#include <unordered_set>

#include "commons/entity.h"
#include "commons/pattern.h"
#include "commons/relationship.h"
#include "pkb/entity/entity_manager.h"
#include "pkb/pattern/pattern_manager.h"
#include "pkb/relationship/relationship_manager.h"

/*
 * Interface for SP -> PKB
 */
class IPKBPopulator {
 protected:
  IPKBPopulator() = default;

 public:
  ~IPKBPopulator() = default;
  virtual void populate(std::vector<Entity *>) = 0;
  virtual void populate(std::vector<Relationship *>) = 0;
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
  virtual EntityPointerUnorderedSet getEntities(EntityType) = 0;
  virtual EntityPointerUnorderedSet getByRelationship(RsType, Entity *, bool) = 0;
  virtual EntityPointerUnorderedSet getByPattern(Entity *, std::string &, bool isSubmatch) = 0;
  virtual EntityPointerUnorderedSet getEntitiesByString(std::string &) = 0;
  virtual std::unordered_map<EntityType, int> getSimpleStats() = 0;

  virtual void LogStatistics() = 0;
};

class PKB : public IPKBPopulator, public IPKBQuerier {
 private:
  EntityManager *entity_manager_;

  RelationshipManager *relationship_manager_;

  PatternManager *pattern_manager_;

 public:
  PKB();

  EntityPointerUnorderedSet getEntities(EntityType) override;
  EntityPointerUnorderedSet getByRelationship(RsType, Entity *, bool) override;
  EntityPointerUnorderedSet getByPattern(Entity *, std::string &, bool isSubmatch) override;
  EntityPointerUnorderedSet getEntitiesByString(std::string &) override;
  std::unordered_map<EntityType, int> getSimpleStats() override;

  void populate(std::vector<Entity *> entities) override;
  void populate(std::vector<Relationship *> relationships) override;
  void populate(std::vector<Pattern *> patterns) override;

  void LogStatistics() override;
};
