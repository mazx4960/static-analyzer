// Copyright 2022 CS3203 Team14. All rights reserved.

#include "query_call.h"

SynonymReference *QueryCall::getReference() const {
  return this->synonym_reference_;
}
void QueryCall::setReference(SynonymReference *synonym_reference) {
  this->synonym_reference_ = synonym_reference;
}
