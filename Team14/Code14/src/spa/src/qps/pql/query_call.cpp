// Copyright 2022 CS3203 Team14. All rights reserved.

#include "query_call.h"

#include <utility>

SelectType SelectCall::getSelectType() const {
  return this->type_;
}
std::string SelectCall::toString() const {
  return "Select " + SelectTypeToString(this->type_);
}

std::vector<ElemReference *> ElemSelect::getReferences() const {
  return this->elem_references_;
}
void ElemSelect::setReferences(std::vector<ElemReference *> elem_references) {
  this->elem_references_ = std::move(elem_references);
}
std::string ElemSelect::toString() const {
  std::string str = SelectCall::toString() + ": ";
  for (auto *ref : getReferences()) {
    str.append(ref->toString() + ", ");
  }
  return str;
}
