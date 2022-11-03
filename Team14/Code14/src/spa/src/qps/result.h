// Copyright 2022 CS3203 Team14. All rights reserved.

#pragma once

#include "qps/pql/query_reference.h"

/**
 * Abstract base class;
 */
class Result {
 protected:
  std::vector<ElemReference *> selected_;
  std::unordered_set<std::string> results_;

 public:
  static Result *Empty();
  static Result *True();
  static Result *False();
  explicit Result(const std::vector<ElemReference *> &selected, const std::unordered_set<std::string> &results)
      : selected_(std::move(selected)), results_(std::move(results)) {
  };
  std::unordered_set<std::string> GetResults() const;
  std::string ToString() const;
};
