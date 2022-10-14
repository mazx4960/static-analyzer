//
// Created by kelvi on 14/10/2022.
//

#include "attr_compare.h"
QueryReference *AttrCompare::getFirst() const {
  return this->first_;
}
QueryReference *AttrCompare::getSecond() const {
  return this->second_;
}
std::string AttrCompare::toString() const {
  return this->first_->toString() + " = " + this->second_->toString();
}
