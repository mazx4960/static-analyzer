// Copyright 2022 CS3203 Team14. All rights reserved.

#include "query_call.h"
#include "commons/helper/vector_comparator.h"

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
bool ElemSelect::operator==(const SelectCall &other) const {
  if (this->GetSelectType() != other.GetSelectType()) {
    return false;
  }
  auto this_references = this->GetReferences();
  auto other_references = static_cast<const ElemSelect &>(other).GetReferences();
  return VectorComparator<ElemReference>::CompareVectors(this_references, other_references);
}
bool BooleanSelect::operator==(const SelectCall &other) const {
  return this->GetSelectType() == other.GetSelectType();
}
