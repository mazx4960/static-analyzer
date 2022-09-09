// Copyright 2022 CS3203 Team14. All rights reserved.

#pragma once

#include <algorithm>
#include <string>
#include <unordered_set>
#include <vector>
#include "qps/pql/query_synonym/query_synonym.h"

class Result {
 private:
  bool is_empty_ = true;

  QuerySynonym syn_;

  std::unordered_set<std::string> results_;

 protected:
  explicit Result(QuerySynonym &syn, std::unordered_set<std::string> &results, bool is_empty)
      : syn_(syn), results_(results), is_empty_(is_empty) {};

 public:
  explicit Result(QuerySynonym &syn, std::unordered_set<std::string> &results)
      : syn_(syn), results_(results), is_empty_(false) {};

  static Result empty(QuerySynonym &);

  [[nodiscard]] bool is_empty() const;
  [[nodiscard]] QuerySynonym get_synonym() const;
  [[nodiscard]] std::unordered_set<std::string> get_results_set() const;
  [[nodiscard]] std::vector<std::string> get_sorted_results_list() const;
};
