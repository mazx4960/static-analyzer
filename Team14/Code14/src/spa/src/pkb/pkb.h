// Copyright 2022 CS3203 Team14. All rights reserved.
#pragma once

#include <string>
#include <unordered_map>
#include <unordered_set>

#include "commons/pkb_query.h"
#include "commons/relationship.h"
#include "commons/result.h"
#include "pkb.h"
#include "pkb/entity/entity_table.h"

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
  virtual Result getResults(PKBQuery &) = 0;
  virtual Result getResults(EntityType, QuerySynonym) = 0;
};

class PKB : public IPKBPopulator, public IPKBQuerier {
 private:
  std::unordered_map<EntityType, EntityTable *> entity_map_;

 public:
  PKB() = default;

  Result getResults(PKBQuery &query) override;
  Result getResults(EntityType type, QuerySynonym synonym) override;

  void populate(std::vector<Entity *> &entities) override;

  // Currently only used for debugging and testing
  int getCount();
};
