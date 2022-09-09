// Copyright 2022 CS3203 Team14. All rights reserved.

#include "query_declaration.h"

QuerySynonym QueryDeclaration::getSynonym() const {
  return this->query_synonym_;
}
bool QueryDeclaration::operator==(const QueryDeclaration &other) const {
  return this->query_synonym_ == other.getSynonym() && this->type_ == other.getType()
  && this->string_ == other.getString() && this->number_ == other.getNumber();
}
DeclarationType QueryDeclaration::getType() const {
  return this->type_;
}
std::string QueryDeclaration::getString() const {
  return this->string_;
}
int QueryDeclaration::getNumber() const {
  return this->number_;
}
