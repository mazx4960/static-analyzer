// Copyright 2022 CS3203 Team14. All rights reserved.

#include "query_declaration.h"

QuerySynonym *QueryDeclaration::getSynonym() const { return this->query_synonym_; }
bool QueryDeclaration::operator==(const QueryDeclaration &other) const {
  if (this->type_ == EntityType::kWildcard || other.getType() == EntityType::kWildcard) { return true; }
  return this->type_ == other.getType() && *(this->query_synonym_) == *(other.getSynonym())
      && this->string_ == other.toString();
}
bool QueryDeclaration::operator==(const QueryDeclaration *other) const {
  if (this->type_ == EntityType::kWildcard || other->getType() == EntityType::kWildcard) { return true; }
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
  std::unordered_set<Entity *, EntityHashFunction, EntityPointerEquality> new_context;
  for (auto *entity_ptr : this->context_) {
    for (auto *other_entity_ptr : other_context) {
      if (*entity_ptr == *other_entity_ptr) { new_context.insert(entity_ptr); }
    }
  }
  this->setContext(new_context);
}

void QueryDeclaration::setContext(std::unordered_set<Entity *, EntityHashFunction, EntityPointerEquality> context) {
  this->context_ = std::move(context);
}
