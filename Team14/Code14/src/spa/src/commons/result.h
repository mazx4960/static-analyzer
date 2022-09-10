// Copyright 2022 CS3203 Team14. All rights reserved.

#pragma once

#include <algorithm>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include "qps/pql/query_synonym/query_synonym.h"
#include "commons/entity.h"

class Result {
 private:
  bool is_empty_ = true;

  QuerySynonym syn_;

  std::unordered_set<std::string> results_;
  std::unordered_map<QuerySynonym, std::unordered_set<Entity*>, QuerySynonymHashFunction> map_result_;

 protected:
  explicit Result(QuerySynonym &syn, std::unordered_set<std::string> &results, bool is_empty)
      : syn_(syn), results_(results), is_empty_(is_empty) {};

 public:
  explicit Result(QuerySynonym &syn, std::unordered_set<std::string> &results)
      : syn_(syn), results_(results), is_empty_(false) {};

  explicit Result(QuerySynonym &syn, std::unordered_map<QuerySynonym, std::unordered_set<Entity*>, QuerySynonymHashFunction> &map_result)
      : syn_(syn), map_result_(map_result) {};

  static Result empty(QuerySynonym &);

  [[nodiscard]] bool is_empty() const;
  [[nodiscard]] QuerySynonym get_synonym() const;
  [[nodiscard]] std::unordered_set<std::string> get_results_set() const;
  [[nodiscard]] std::vector<std::string> get_sorted_results_list() const;
  [[nodiscard]] std::unordered_map<QuerySynonym, std::unordered_set<Entity*>, QuerySynonymHashFunction> get_result_map();
};
