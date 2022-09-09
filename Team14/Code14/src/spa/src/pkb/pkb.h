// Copyright 2022 CS3203 Team14. All rights reserved.
#pragma once

#include <string>
#include <unordered_map>
#include <unordered_set>

#include "pkb.h"
#include "commons/relationship.h"
#include "pkb/entity/pkb_query.h"
#include "pkb/entity/result.h"
#include "pkb/entity/entity_store.h"

/*
 * Interface for PKB Saver
 */
class IPKBSaver {
 protected:
  IPKBSaver() = default;
 public:
  ~IPKBSaver() = default;
  virtual void save(std::vector<Entity *> &) = 0;
};

/*
 * Interface for PKB Getter
 */
class IPKBGetter {
 protected:
  IPKBGetter() = default;
 public:
  ~IPKBGetter() = default;
  virtual Result get(PKBEntityQuery &) = 0;
  virtual Result get(EntityType, QuerySynonym) = 0;
};

class PKB : public IPKBSaver, public IPKBGetter {
 private:
  std::unordered_map<EntityType, EntityStore *> entity_storage_map_;

 public:
  PKB() = default;

  Result get(PKBEntityQuery &query) override;
  Result get(EntityType type, QuerySynonym synonym) override;

  void save(std::vector<Entity *> &entities) override;

  // Currently only used for debugging and testing
  int getCount();
};
