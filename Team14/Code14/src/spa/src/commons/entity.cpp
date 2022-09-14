// Copyright 2022 CS3203 Team14. All rights reserved.

#include "entity.h"
#include "spdlog/spdlog.h"

Entity::Entity(EntityType type, std::string name) {
  this->type_ = type;
  this->name_ = std::move(name);
}
EntityType Entity::GetType() const {
  return this->type_;
}
std::string Entity::GetName() const {
  return this->name_;
}
bool Entity::operator==(const Entity &other) const {
  return this->type_ == other.GetType() && this->name_ == other.GetName();
}
size_t Entity::GetHash() const {
  size_t hash = 0;
  HashCombine<EntityType>::hash_combine(hash, this->GetType());
  HashCombine<std::string>::hash_combine(hash, this->GetName());
  spdlog::info("String hash: {}, EntityType hash: {} Entity hash: {}",
               std::hash<std::string>{}(this->name_),
               std::hash<EntityType>{}(this->type_),
               hash);
  return hash;
}
std::string Entity::ToString() {
  std::string type_string;
  std::string name_string = this->name_;
  switch (this->type_) {
    case EntityType::kProcedure:type_string = "procedure";
      break;
    case EntityType::kVariable:type_string = "variable";
      break;
    case EntityType::kConstant:type_string = "constant";
      break;
    case EntityType::kStatement: {
      auto *statement_entity = static_cast<StatementEntity *>(this);
      name_string = std::to_string(statement_entity->GetStmtNo());
      switch (statement_entity->GetType()) {
        case EntityType::kAssignStmt:type_string = "assign";
          break;
        case EntityType::kCallStmt:type_string = "call";
          break;
        case EntityType::kIfStmt:type_string = "if";
          break;
        case EntityType::kPrintStmt:type_string = "print";
          break;
        case EntityType::kReadStmt:type_string = "read";
          break;
        case EntityType::kWhileStmt:type_string = "while";
          break;
        default:type_string = "unknown statement";
          break;
      }
    }
    default: break; // string entity types should not reach this stage
  }
  return type_string + ": " + name_string;
}

StatementEntity::StatementEntity(int stmt_no, EntityType entity_type)
    : Entity(entity_type, std::to_string(stmt_no)) {
  this->stmt_no_ = stmt_no;
}
int StatementEntity::GetStmtNo() const {
  return this->stmt_no_;
}
bool StatementEntity::operator==(const StatementEntity &other) const {
  return this->type_ == other.GetType() && this->stmt_no_ == other.GetStmtNo();
}
