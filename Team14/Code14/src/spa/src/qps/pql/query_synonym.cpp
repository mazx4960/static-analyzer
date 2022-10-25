// Copyright 2022 CS3203 Team14. All rights reserved.

#include "query_synonym.h"

#include <utility>

std::string QuerySynonym::GetName() const {
  return this->name_;
}
EntityType QuerySynonym::GetEntityType() const {
  return this->type_;
}
std::string QuerySynonym::ToString() const {
  return EntityTypeToString(this->GetEntityType()) + ": " + this->GetName();
}
bool QuerySynonym::operator==(const QuerySynonym &other) const {
  return this->name_ == other.name_ && this->type_ == other.type_;
}
bool QuerySynonym::operator!=(const QuerySynonym &other) const {
  return !(*this == other);
}
