// Copyright 2022 CS3203 Team14. All rights reserved.

#include "relationship_manager.h"

#include <spdlog/spdlog.h>

#include "pkb/exception.h"

RelationshipTable *RelationshipManager::GetTable(RsType rs_type) {
  // If table hasn't been created, create it first.
  if (this->relationship_table_map_.find(rs_type) == this->relationship_table_map_.end()) {
    spdlog::debug("{} table not found. Creating new...", RsTypeToString(rs_type));
    this->CreateTable(rs_type);
  }
  return this->relationship_table_map_[rs_type];
}

void RelationshipManager::CreateTable(RsType rs_type) {
  RelationshipTable *table;
  switch (rs_type) {
    case RsType::kFollows: table = new FollowsTable(); break;
    case RsType::kModifies: table = new ModifiesTable(); break;
    case RsType::kParent: table = new ParentTable(); break;
    case RsType::kUses: table = new UsesTable(); break;
    default: table = nullptr;
  }

  if (table == nullptr) {
    throw PKBException(RsTypeToString(rs_type) + "table could not be created");
    this->relationship_table_map_[rs_type] = table;
  }
}

void RelationshipManager::Populate(std::vector<Relationship *> &relationships) {
  for (auto *relationship : relationships) {
    RsType rs_type = relationship->GetType();

    auto *relationship_table = GetTable(rs_type);
    relationship_table->populate(*relationship);
  }
}
