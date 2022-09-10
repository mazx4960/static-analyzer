#include "result.h"

#include "spdlog/spdlog.h"

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

std::unordered_set<std::string> Result::get_results_set() const {
  return this->results_;
}

std::vector<std::string> Result::get_sorted_results_list() const {
  std::vector<std::string> result_vector(this->results_.begin(), this->results_.end());
  std::sort(result_vector.begin(), result_vector.end());
  return result_vector;
}

std::unordered_set<Entity *> Result::get_results_entity_set() {
  return this->entity_set_results_;
}
