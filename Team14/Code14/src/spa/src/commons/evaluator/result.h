#pragma once

#include <string>
#include <vector>
#include "qps/pql/query_synonym/query_synonym.h"
class Result {
 private:
  QuerySynonym syn_;

  std::vector<std::string> results_;

 public:
  explicit Result(QuerySynonym &syn, std::vector<std::string> &results) : syn_(syn), results_(results) {};

};
