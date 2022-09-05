#pragma once

#include <string>
#include <unordered_set>
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

  bool is_empty();
  QuerySynonym get_synonym();
  std::unordered_set<std::string> get_results();
};
