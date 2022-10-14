// Copyright 2022 CS3203 Team14. All rights reserved.

#pragma once

#include <string>
#include <utility>
#include "query_reference.h"

class AttrCompare {
 private:
  QueryReference *first_;
  QueryReference *second_;
 protected:
  explicit AttrCompare(QueryReference *first, QueryReference *second) : first_(first), second_(second) {
  };
 public:
  [[nodiscard]] std::string toString() const;
  [[nodiscard]] QueryReference *getFirst() const;
  [[nodiscard]] QueryReference *getSecond() const;
};