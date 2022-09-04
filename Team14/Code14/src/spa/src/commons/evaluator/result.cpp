#include "result.h"

Result Result::empty(QuerySynonym &syn) {
  std::vector<std::string> empty;
  return Result(syn, empty, true);
}

bool Result::is_empty() {
  return this->is_empty_;
}

QuerySynonym Result::get_synonym() {
  return this->syn_;
}

std::vector<std::string> Result::get_results_list() {
  return this->results_;
}

