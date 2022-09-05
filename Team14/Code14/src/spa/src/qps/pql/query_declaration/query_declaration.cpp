// Copyright 2022 CS3203 Team14. All rights reserved.

#include "query_declaration.h"

VariableEntity QueryDeclaration::getEntity() const {
  return this->entity_;
}
QuerySynonym QueryDeclaration::getSynonym() const {
  return this->synonym_;
}
bool QueryDeclaration::operator==(const QueryDeclaration &other) const {
  return this->getSynonym() == other.getSynonym();
}

size_t QueryDeclarationHashFunction::operator()(const QueryDeclaration &query_declaration) const {
  return std::hash<std::string>()(query_declaration.getSynonym().getSynonym());
}
