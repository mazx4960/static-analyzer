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
  return this->synonym_string_;
}

ElemResult::ElemResult(std::vector<ElemReference *> &elem_refs, SubqueryResult &table) : elem_refs_(elem_refs) {
  int missing_count = this->numSynonymsNotInTable(table);
  if (missing_count > 0) {
    throw ResultCreationError(std::to_string(missing_count) + " synonyms not found in table");
  }

  std::vector<ResultRow> rows = table.GetRows();
  this->results_.reserve(rows.size());
  for (auto row : rows) {
    std::string row_string;
    for (int i = 0; i < elem_refs_.size(); i++) {
      auto *elem_ref = this->elem_refs_[i];
      if (i > 0) {
        row_string += " ";
      }

      auto *entity = row[elem_ref->getSynonym()];
      if (elem_ref->getRefType() == ReferenceType::kAttr) {
        row_string += static_cast<AttrReference *>(elem_ref)->getAttribute(entity);
      } else {
        row_string += entity->GetValue();
      }
    }
    this->results_.insert(row_string);
  }
}

int ElemResult::numSynonymsNotInTable(SubqueryResult &table) {
  int count = 0;
  for (auto *elem_ref : this->elem_refs_) {
    if (!table.Uses(elem_ref->getSynonym())) {
      count++;
    }
  }
  return count;
}

std::string ElemResult::get_synonyms() const {
  std::string synonyms_string;
  int size = this->elem_refs_.size();
  for (int i = 0; i < size; i++) {
    auto *ref = this->elem_refs_[i];
    synonyms_string += ref->toString() + (i < size - 1 ? ", " : "");
  }
  return "{ " + synonyms_string + " }";
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
