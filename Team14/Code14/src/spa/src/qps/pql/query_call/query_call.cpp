// Copyright 2022 CS3203 Team14. All rights reserved.

#include "query_call.h"

CallType QueryCall::getType() {
  return this->type_;
}
QueryDeclaration* QueryCall::getDeclaration() {
  return this->query_declaration_;
}
std::vector<QueryClause> QueryCall::getClauseVector() {
  return this->clause_vector_;
}
