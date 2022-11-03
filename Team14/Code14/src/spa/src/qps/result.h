// Copyright 2022 CS3203 Team14. All rights reserved.

#pragma once

#include <list>
#include "qps/pql/query_reference.h"

/**
 * Abstract base class;
 */
class Result {
 protected:
  std::vector<ElemReference *> selected_;
  std::list<std::string> results_;

 public:
  static Result *Empty();
  static Result *True();
  static Result *False();
  explicit Result(const std::vector<ElemReference *> &selected, const std::list<std::string> &results)
      : selected_(std::move(selected)), results_(std::move(results)) {
  };
  [[nodiscard]] std::list<std::string> GetResults() const;
  [[nodiscard]] std::string ToString() const;
};
