// Copyright 2022 CS3203 Team14. All rights reserved.

#include "query_call.h"

SelectType SelectCall::GetSelectType() const {
  return this->type_;
}
std::string SelectCall::ToString() const {
  return "Select " + SelectTypeToString(this->type_);
}

std::vector<ElemReference *> ElemSelect::GetReferences() const {
  return this->elem_references_;
}
std::string ElemSelect::ToString() const {
  std::string str = SelectCall::ToString() + ": ";
  for (auto *ref : GetReferences()) {
    str.append(ref->toString() + ", ");
  }
  return str;
}
