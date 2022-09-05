#include "result.h"

Result Result::empty(QuerySynonym &syn) {
  std::unordered_set < std::string > empty;
  return Result(syn, empty, true);
}

bool Result::is_empty() {
  return this->is_empty_;
}

QuerySynonym Result::get_synonym() {
  return this->syn_;
}

std::unordered_set<std::string> Result::get_results() {
  return this->results_;
}

