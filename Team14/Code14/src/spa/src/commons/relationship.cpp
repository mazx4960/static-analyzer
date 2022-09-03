// Copyright 2022 CS3203 Team14. All rights reserved.
#include "relationship.h"

#include <utility>

#include "entity.h"

Relationship::Relationship(RsType type, Entity first, Entity second) : type(type),
                                                                       first(std::move(first)),
                                                                       second(std::move(second)) {}
bool Relationship::operator==(const Relationship& other) const {
  return this->type == other.type && this->first == other.first && this->second == other.second;
}
RsType Relationship::GetType() {
  return this->type;
}
Entity Relationship::GetFirst() {
  return this->first;
}
Entity Relationship::GetSecond() {
  return this->second;
}
