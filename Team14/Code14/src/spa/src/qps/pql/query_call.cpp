// Copyright 2022 CS3203 Team14. All rights reserved.

#include "query_call.h"

CallType QueryCall::getType() const {
  return this->type_;
}
QueryDeclaration *QueryCall::getDeclaration() const {
  return this->query_declaration_;
}
std::vector<QueryClause*> QueryCall::getClauseVector() const {
  return this->clause_vector_;
}
bool QueryCall::hasSubClauses() const {
  return !this->getClauseVector().empty();
}
