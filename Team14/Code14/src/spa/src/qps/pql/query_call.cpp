// Copyright 2022 CS3203 Team14. All rights reserved.

#include "query_call.h"

#include <utility>

std::vector<ElemReference *> QueryCall::getReferences() const {
  return this->elem_references_;
}
void QueryCall::setReferences(std::vector<ElemReference *> elem_references) {
  this->elem_references_ = std::move(elem_references);
}
CallType QueryCall::getCallType() const {
  return this->type_;
}
std::string SelectCall::toString() const {
  std::string str = "Select {";
  for (auto *ref : getReferences()) {
    str.append(ref->toString() + ", ");
  }
  str.append("}");
  return str;
}
