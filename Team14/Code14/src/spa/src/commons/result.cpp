#include "result.h"

Result *Result::empty() {
  std::unordered_set<Entity *> empty_set;
  auto *empty_synonym = new QuerySynonym("empty");
  return new Result(empty_synonym, &empty_set);
}

Result *Result::empty(QuerySynonym *synonym) {
  std::unordered_set<Entity *> empty_set;
  return new Result(synonym, &empty_set);
}

bool Result::is_empty() const {
  return this->results_->empty();
}

QuerySynonym *Result::get_synonym() const {
  return this->synonym_;
}

std::unordered_set<Entity *> *Result::get_results_set() const {
  return this->results_;
}

std::vector<Entity *> *Result::get_sorted_results_list() const {
  std::vector<Entity *> *result_vector = new std::vector<Entity *>(this->results_->begin(), this->results_->end());
  std::sort(result_vector->begin(), result_vector->end());
  return result_vector;
}
