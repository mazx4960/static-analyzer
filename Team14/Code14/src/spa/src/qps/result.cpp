#include "result.h"

Result::Result(QuerySynonym *synonym, const EntityPointerUnorderedSet &results_set) : synonym_(std::move(synonym)) {
  this->results_.reserve(results_set.size());
  for (auto *entity : results_set) {
    this->results_.insert(entity->GetValue());
  }
}

Result::Result(QuerySynonym *synonym, std::unordered_set<std::string> results_set)
    : synonym_(std::move(synonym)), results_(std::move(results_set)) {};

Result *Result::empty() {
  auto *empty_synonym = new QuerySynonym("empty");
  return Result::empty(empty_synonym);
}

Result *Result::empty(QuerySynonym *synonym) {
  std::unordered_set<std::string> empty_set;
  return new Result(std::move(synonym), empty_set);
}

Result *Result::semanticError() {
  auto *synonym = new QuerySynonym("semantic_error");
  std::unordered_set<std::string> error_set = {"SemanticError"};
  return new Result(synonym, error_set);
}
Result *Result::syntacticError() {
  auto *synonym = new QuerySynonym("syntactic_error");
  std::unordered_set<std::string> error_set = {"SyntaxError"};
  return new Result(synonym, error_set);
}

bool Result::is_empty() const {
  return this->results_.empty();
}

QuerySynonym *Result::get_synonym() const {
  return this->synonym_;
}

std::unordered_set<std::string> Result::get_results_set() const {
  return this->results_;
}

std::vector<std::string> Result::get_sorted_results_string_list() const {
  std::vector<std::string> result_string_vector;
  result_string_vector.reserve(this->results_.size());
  std::copy(this->results_.begin(), this->results_.end(), std::back_inserter(result_string_vector));
  std::sort(result_string_vector.begin(), result_string_vector.end());
  return result_string_vector;
}
int Result::size() const {
  return this->results_.size();
}
