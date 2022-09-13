// Copyright 2022 CS3203 Team14. All rights reserved.

#include "query_synonym.h"
#include "spdlog/spdlog.h"

#include <utility>

QuerySynonym::QuerySynonym(std::string synonym) {
  this->synonym_ = synonym;
}
std::string QuerySynonym::getSynonym() const {
  return this->synonym_;
}
bool QuerySynonym::operator==(const QuerySynonym &other) const {
  spdlog::info("Comparing reference {} with {}", this->synonym_, other.getSynonym());
  return this->synonym_ == other.getSynonym();
}
size_t QuerySynonymHashFunction::operator()(const QuerySynonym &synonym) const {
  return std::hash<std::string>()(synonym.getSynonym());
}
size_t QuerySynonymHashFunction::operator()(const QuerySynonym *synonym) const {
  return std::hash<std::string>()(synonym->getSynonym());
}
