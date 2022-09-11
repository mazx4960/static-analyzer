// Copyright 2022 CS3203 Team14. All rights reserved.

#pragma once

#include <algorithm>
#include <string>
#include <unordered_set>
#include <vector>
#include "commons/entity.h"
#include "qps/pql/query_synonym/query_synonym.h"

class Result {
 private:
  QuerySynonym synonym_;

  std::unordered_set<Entity *> results_;

 public:
  explicit Result(QuerySynonym &synonym, std::unordered_set<Entity *> &results_set)
      : synonym_(synonym), results_(results_set) {};

  static Result empty(QuerySynonym &synonym);

  [[nodiscard]] bool is_empty() const;
  [[nodiscard]] QuerySynonym get_synonym() const;
  [[nodiscard]] std::unordered_set<Entity *> get_results_set() const;
  [[nodiscard]] std::vector<Entity *> get_sorted_results_list() const;
};
