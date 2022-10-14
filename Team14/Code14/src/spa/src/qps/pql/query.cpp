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
std::string Query::toString() const {
  std::string str;
  str.append("Declarations: {");
  for (auto *declaration : synonym_declarations_) {
    str.append(declaration->toString() + "; ");
  }
  str.append("}, ");
  str.append("QueryCall: {");
  str.append(query_call_->toString());
  str.append("}, ");
  str.append("Clauses: {");
  for (auto *clause : query_clauses_) {
    str.append(clause->toString() + " ");
  }
  str.append("}");
  return str;
}
