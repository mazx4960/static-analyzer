// Copyright 2022 CS3203 Team14. All rights reserved.

#include "query.h"

SynonymSet Query::getSynonymDeclarations() const {
  return this->synonym_declarations_;
}
SelectCall *Query::getQueryCall() const {
  return this->query_call_;
}
ClauseVector Query::getClauses() const {
  return this->query_clauses_;
}
std::string Query::toString() const {
  std::string str;
  str.append("Declarations: {");
  for (auto *declaration : synonym_declarations_) {
    str.append(declaration->ToString() + "; ");
  }
  str.append("}, QueryCall:");
  str.append(query_call_->ToString());
  str.append(", Clauses: {");
  for (auto *clause : query_clauses_) {
    str.append(clause->toString() + " ");
  }
  str.append("}");
  return str;
}
