#include "result.h"

#include <utility>

Result::Result(ElemReference *elem_ref, const EntityPointerUnorderedSet &results_set)
    : elem_refs_({std::move(elem_ref)}) {
  this->results_.reserve(results_set.size());
  for (auto *entity : results_set) {
    this->results_.insert(entity->GetValue());
  }
}

Result::Result(ElemReference *elem_ref, std::unordered_set<std::string> results_set)
    : elem_refs_({std::move(elem_ref)}), results_(std::move(results_set)) {
};

Result *Result::empty() {
  return Result::empty({});
}

Result *Result::empty(std::vector<ElemReference *> elem_refs) {
  std::unordered_set<std::string> empty_set;
  return new Result(std::move(elem_refs), SubqueryResult::Empty({}));
}

Result *Result::semanticError() {
  auto *synonym = new QuerySynonym("semantic_error");
  std::unordered_set<std::string> error_set = {"SemanticError"};
  return new Result(new SynonymReference(synonym), error_set);
}
Result *Result::syntacticError() {
  auto *synonym = new QuerySynonym("syntactic_error");
  std::unordered_set<std::string> error_set = {"SyntaxError"};
  return new Result(new SynonymReference(synonym), error_set);
}

bool Result::is_empty() const {
  return this->results_.empty();
}

std::string Result::get_synonyms() const {
  std::string synonyms_string;
  for (auto *elem_ref : elem_refs_) {
    synonyms_string += elem_ref->toString() + ", ";
  }
  return synonyms_string;
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
Result::Result(std::vector<ElemReference *> elem_refs, const SubqueryResult &table) : elem_refs_(std::move(elem_refs)) {
  std::vector<ResultRow> rows = table.GetRows();
  this->results_.reserve(rows.size());
  for (auto row : rows) {
    std::string row_string;
    if (elem_refs_.size() > 1) {
      row_string += "<";
    }
    for (int i = 0; i < elem_refs_.size(); i++) {
      auto *elem_ref = elem_refs_[i];
      if (i > 0) {
        row_string += ", ";
      }

      row_string += row[elem_ref->getSynonym()]->GetValue();
    }
    if (elem_refs_.size() > 1) {
      row_string += ">";
    }
    this->results_.insert(row_string);
  }
}
