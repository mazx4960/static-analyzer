// Copyright 2022 CS3203 Team14. All rights reserved.

#include "query.h"

std::vector<SynonymReference *> Query::getSynonymDeclarations() const {
  return this->synonym_declarations_;
}
QueryCall *Query::getQueryCall() const {
  return this->query_call_;
}
Clauses Query::getClauses() const {
  return this->query_clauses_;
}