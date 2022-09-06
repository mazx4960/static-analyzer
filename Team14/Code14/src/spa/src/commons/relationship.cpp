// Copyright 2022 CS3203 Team14. All rights reserved.
#include "relationship.h"

#include <utility>

#include "entity.h"

Relationship::Relationship(RsType type, Entity *first, Entity *second) : type_(type),
                                                                         first_(std::move(first)),
                                                                         second_(std::move(second)) {}
bool Relationship::operator==(const Relationship &other) const {
  return this->type_ == other.type_ && this->first_ == other.first_ && this->second_ == other.second_;
}
RsType Relationship::GetType() {
  return this->type_;
}
Entity *Relationship::GetFirst() {
  return this->first_;
}
Entity *Relationship::GetSecond() {
  return this->second_;
}
std::string Relationship::ToString() {
  std::string type_string;
  switch (this->type_) {
    case RsType::kFollows:type_string = "Follows";
      break;
    case RsType::kParent:type_string = "Parent";
      break;
    case RsType::kUses:type_string = "Uses";
      break;
    case RsType::kModifies:type_string = "Modifies";
      break;
    default:type_string = "unknown";
      break;
  }
  return type_string + " (" + this->first_->ToString() + ", " + this->second_->ToString() + ")";
}
