// Copyright 2022 CS3203 Team14. All rights reserved.

#include "relationship_manager.h"

#include <spdlog/spdlog.h>

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
    case RsType::kParent: table = new ParentTable(); break;
    case RsType::kModifies: table = new ModifiesTable(); break;
    case RsType::kUses: table = new UsesTable(); break;
    default: table = nullptr;
  }

  if (table == nullptr) { throw PKBException(RsTypeToString(rs_type) + " table could not be created"); }
  this->relationship_table_map_[rs_type] = table;
}

void RelationshipManager::Populate(std::vector<Relationship *> &relationships) {
  for (auto *relationship : relationships) {
    RsType rs_type = relationship->GetType();

    auto *relationship_table = GetTable(rs_type);
    relationship_table->populate(*relationship);
  }
}

EntityPointerUnorderedSet RelationshipManager::Get(RsType rs_type, Entity *entity, bool is_inverse) {
  spdlog::debug("Retrieving all entities that {} {}, inverse = {}", RsTypeToString(rs_type), entity->GetValue(),
                is_inverse);
  auto matches = EntityPointerUnorderedSet();
  switch (rs_type) {
    case RsType::kFollowsAll: {
      matches = this->GetAll(RsType::kFollows, entity, is_inverse);
      break;
    }
    case RsType::kParentAll: {
      matches = this->GetAll(RsType::kParent, entity, is_inverse);
      break;
    }
    default: {
      auto *relationship_table = GetTable(rs_type);
      matches = relationship_table->get(entity, is_inverse);
    }
  }
  std::string result_string;
  for (auto *match : matches) { result_string += match->GetValue() + " "; }
  spdlog::debug("Result: {}", result_string);
  return matches;
}

EntityPointerUnorderedSet RelationshipManager::GetAll(RsType rs_type, Entity *entity, bool is_inverse) {
  auto matches = EntityPointerUnorderedSet();
  auto *relationship_table = GetTable(rs_type);

  std::queue<Entity *> queue;
  queue.push(entity);
  while (!queue.empty()) {
    auto *current = queue.front();
    auto sub_matches = relationship_table->get(current, is_inverse);
    for (auto *sub_match : sub_matches) {
      if (matches.find(sub_match) == matches.end()) {
        matches.insert(sub_match);
        queue.push(sub_match);
      }
    }
    queue.pop();
  }
  return matches;
}
