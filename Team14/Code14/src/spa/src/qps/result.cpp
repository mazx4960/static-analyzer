#include "result.h"

#include <utility>

Result::Result(QuerySynonym *synonym, const EntityPointerUnorderedSet &results_set) : synonyms_({std::move(synonym)}) {
  this->results_.reserve(results_set.size());
  for (auto *entity : results_set) {
    this->results_.insert(entity->GetValue());
  }
}

Result::Result(QuerySynonym *synonym, std::unordered_set<std::string> results_set)
    : synonyms_({std::move(synonym)}), results_(std::move(results_set)) {
};

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

std::string Result::get_synonyms() const {
  std::string synonyms_string;
  for (auto *synonym : synonyms_) {
    synonyms_string += synonym->toString() + ", ";
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
Result::Result(std::vector<QuerySynonym *> synonyms, const SubqueryResult& table) : synonyms_(std::move(synonyms)) {
  std::vector<ResultRow> rows = table.GetRows();
  this->results_.reserve(rows.size());
  for (auto row : rows) {
    std::string row_string;
    if (synonyms_.size() > 1) {
      row_string += "<";
    }
    for (int i = 0; i < synonyms_.size(); i++) {
      auto *synonym = synonyms_[i];
      if (i > 0) {
        row_string += ", ";
      }
      row_string += row[synonym]->GetValue();
    }
    if (synonyms_.size() > 1) {
      row_string += ">";
    }
    this->results_.insert(row_string);
  }
}
