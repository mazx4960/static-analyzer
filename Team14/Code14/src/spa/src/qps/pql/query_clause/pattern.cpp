// Copyright 2022 CS3203 Team14. All rights reserved.

#include "pattern.h"

#include <utility>

Pattern::Pattern(Entity first, Entity second): first_(std::move(first)), second_(std::move(second)) {}
bool Pattern::operator==(const Pattern& other) const {
  return this->type_ == other.type_ && this->first_ == other.first_ && this->second_ == other.second_;
}
PatternType Pattern::GetType() {
  return this->type_;
}
Entity Pattern::GetFirst() {
  return this->first_;
}
Entity Pattern::GetSecond() {
  return this->second_;
}
