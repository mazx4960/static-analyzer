#pragma once

#include <string>
#include <vector>
#include "qps/pql/query_synonym/query_synonym.h"
class Result {
 private:
  bool is_empty_ = true;

  QuerySynonym syn_;

  std::vector<std::string> results_;

 protected:
  explicit Result(QuerySynonym &syn, std::vector<std::string> &results, bool is_empty)
      : syn_(syn), results_(results), is_empty_(is_empty) {};

 public:
  explicit Result(QuerySynonym &syn, std::vector<std::string> &results)
      : syn_(syn), results_(results), is_empty_(false) {};

  static Result empty(QuerySynonym &);

  bool is_empty();
  QuerySynonym get_synonym();
  std::vector<std::string> get_results_list();
};
