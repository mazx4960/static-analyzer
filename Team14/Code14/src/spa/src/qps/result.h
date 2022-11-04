// Copyright 2022 CS3203 Team14. All rights reserved.

#pragma once

#include <unordered_set>
#include <vector>
#include "qps/pql/query_reference.h"

class Result {
 private:
  inline static const std::string kBooleanTrue = "TRUE";

  inline static const std::string kBooleanFalse = "FALSE";

 protected:
  std::vector<ElemReference *> selected_;

  std::unordered_set<std::string> results_;

 public:
  Result(const std::vector<ElemReference *> &selected, const std::unordered_set<std::string> &results)
      : selected_(std::move(selected)), results_(std::move(results)) {
  };
  static Result *Empty();
  static Result *True();
  static Result *False();
  
  [[nodiscard]] std::unordered_set<std::string> GetResults() const;
  [[nodiscard]] std::string ToString() const;
};
