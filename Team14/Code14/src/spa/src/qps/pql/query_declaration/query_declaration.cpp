// Copyright 2022 CS3203 Team14. All rights reserved.

#include "query_declaration.h"

VariableEntity QueryDeclaration::getEntity() const {
  return this->entity_;
}
QuerySynonym QueryDeclaration::getSynonym() const {
  return this->synonym_;
}
