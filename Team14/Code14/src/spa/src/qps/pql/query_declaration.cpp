// Copyright 2022 CS3203 Team14. All rights reserved.

#include "query_declaration.h"

QuerySynonym *QueryDeclaration::getSynonym() const { return this->query_synonym_; }
bool QueryDeclaration::operator==(const QueryDeclaration &other) const {
  if (this->type_ == EntityType::kWildcardEnt || other.getType() == EntityType::kWildcardEnt ||
  this->type_ == EntityType::kWildcardStmt || other.getType() == EntityType::kWildcardStmt) { return true; }
  return this->type_ == other.getType() && *(this->query_synonym_) == *(other.getSynonym())
      && this->string_ == other.toString();
}
bool QueryDeclaration::operator==(const QueryDeclaration *other) const {
  if (this->type_ == EntityType::kWildcardEnt || other->getType() == EntityType::kWildcardEnt ||
  this->type_ == EntityType::kWildcardStmt || other->getType() == EntityType::kWildcardStmt) { return true; }
  return this->type_ == other->getType() && this->query_synonym_ == other->getSynonym()
      && this->string_ == other->toString();
}
EntityType QueryDeclaration::getType() const { return this->type_; }
std::string QueryDeclaration::toString() const { return this->string_; }
std::unordered_set<Entity *, EntityHashFunction, EntityPointerEquality> QueryDeclaration::getContext() const {
  return this->context_;
}

void QueryDeclaration::removeEntityFromContext(Entity *entity) { this->context_.erase(entity); }

void QueryDeclaration::intersectContext(
    const std::unordered_set<Entity *, EntityHashFunction, EntityPointerEquality> &other_context) {
  for (auto *entity:context_) {
    if (other_context.find(entity) == other_context.end()) {
      context_.erase(entity);
    }
  }
}

void QueryDeclaration::setContext(std::unordered_set<Entity *, EntityHashFunction, EntityPointerEquality> context) {
  this->context_ = std::move(context);
}
