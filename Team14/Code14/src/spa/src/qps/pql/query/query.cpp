// Copyright 2022 CS3203 Team14. All rights reserved.

#include "query.h"
std::vector<QueryDeclaration*> Query::getDeclarations() const {
  return this->query_declarations_;
}
QueryCall Query::getQueryCall() const {
  return this->query_call_;
}
