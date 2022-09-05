#include "result.h"

Result Result::empty(QuerySynonym &syn) {
  std::unordered_set<std::string> empty;
  return Result(syn, empty, true);
}

[[maybe_unused]] bool Result::is_empty() const {
  return this->is_empty_;
}

QuerySynonym Result::get_synonym() const {
  return this->syn_;
}

std::unordered_set<std::string> Result::get_results() const {
  return this->results_;
}

