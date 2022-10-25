// Copyright 2022 CS3203 Team14. All rights reserved.

#include "query_synonym.h"

#include <utility>

QuerySynonym *QuerySynonym::empty() {
  return new QuerySynonym("");
}
std::string QuerySynonym::toString() const {
  return this->synonym_;
}
bool QuerySynonym::operator==(const QuerySynonym &other) const {
  return this->synonym_ == other.toString();
}
bool QuerySynonym::operator!=(const QuerySynonym &other) const {
  return !(*this == other);
}
