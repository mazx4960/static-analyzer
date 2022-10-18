// Copyright 2022 CS3203 Team14. All rights reserved.

#include "entity.h"

#include "spdlog/spdlog.h"

Entity::Entity(EntityType type, std::string value) {
  this->type_ = type;
  this->value_ = std::move(value);
}
EntityType Entity::GetType() const {
  return this->type_;
}
std::string Entity::GetValue() const {
  return this->value_;
}
bool Entity::operator==(const Entity &other) const {
  return this->type_ == other.GetType() && this->value_ == other.GetValue();
}
size_t Entity::GetHash() const {
  size_t hash = 0;
  HashCombine<EntityType>::hash_combine(hash, this->GetType());
  HashCombine<std::string>::hash_combine(hash, this->GetValue());
  return hash;
}
std::string Entity::ToString() {
  std::string name_string = this->value_;
  return EntityTypeToString(this->type_) + ": " + name_string;
}

std::string StmtEntity::GetStmtNo() const {
  return this->stmt_no_;
}
std::string StmtEntity::GetAttr() const {
  return this->value_;
}
std::string StmtEntity::ToString() {
  return EntityTypeToString(this->type_) + ": " + this->stmt_no_ + " (" + this->value_ + ")";
}

