#pragma once

#include <string>
#include <vector>
#include "qps/qpl/synonym/synonym.h"
class Result {
 private:
  Synonym syn_;

  std::vector<std::string> results_;

 public:
  explicit Result(Synonym &syn, std::vector<std::string> &results) : syn_(syn), results_(results) {};

};
