// Copyright 2022 CS3203 Team14. All rights reserved.

#pragma once

#include <algorithm>
#include <iterator>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

#include "commons/entity.h"
#include "qps/pql/query_synonym.h"
#include "qps/query_evaluator/subquery_result.h"

using EntityPointerUnorderedSet = std::unordered_set<Entity *, EntityHashFunction, EntityPointerEquality>;

class Result {
 private:
  std::vector<ElemReference *> elem_refs_;

  std::unordered_set<std::string> results_;

 public:
  Result(ElemReference *, const EntityPointerUnorderedSet &);

  Result(ElemReference *, std::unordered_set<std::string>);

  Result(std::vector<ElemReference *>, const SubqueryResult &);

  static Result *empty();
  static Result *empty(std::vector<ElemReference *>);

  static Result *semanticError();
  static Result *syntacticError();

  [[nodiscard]] bool is_empty() const;
  [[nodiscard]] std::string get_synonyms() const;
  [[nodiscard]] std::unordered_set<std::string> get_results_set() const;
  [[nodiscard]] std::vector<std::string> get_sorted_results_string_list() const;
  [[nodiscard]] int size() const;
};
