#include "result.h"

Result *Result::empty() {
  return new EmptyResult();
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

bool Result::is_empty() const {
  return this->results_.empty();
}

int Result::size() const {
  return this->results_.size();
}

std::string EmptyResult::get_synonyms() const {
  return "";
}

ElemResult::ElemResult(std::vector<ElemReference *> &elem_refs, SubqueryResult &table) : elem_refs_(elem_refs) {
  std::vector<ResultRow> rows = table.GetRows();
  this->results_.reserve(rows.size());
  for (auto row : rows) {
    std::string row_string;
    for (int i = 0; i < elem_refs_.size(); i++) {
      auto *elem_ref = this->elem_refs_[i];
      if (i > 0) {
        row_string += " ";
      }

      row_string += row[elem_ref->getSynonym()]->GetValue();
    }
    this->results_.insert(row_string);
  }
}

std::string ElemResult::get_synonyms() const {
  std::string synonyms_string;
  for (auto *elem_ref : elem_refs_) {
    synonyms_string += elem_ref->toString() + ", ";
  }
  return synonyms_string;
}

BooleanResult::BooleanResult(bool has_results) {
  if (has_results) {
    this->results_ = true_set_;
  } else {
    this->results_ = false_set_;
  }
}

std::string BooleanResult::get_synonyms() const {
  return this->synonym_string_;
}
