#include "result_projector.h"
#include "spdlog/spdlog.h"

ResultProjector *ResultProjector::getProjector(SelectCall *select_call, std::vector<SubqueryResult> &subquery_results) {
  switch (select_call->getSelectType()) {
    case SelectType::kElem: {
      spdlog::debug("Creating projector for Elem select");
      auto *elem_select_call = static_cast<ElemSelect *>(select_call);
      std::vector<ElemReference *> called_references = elem_select_call->getReferences();
      return new SelectProjector(called_references, subquery_results);
    }
    case SelectType::kBoolean: {
      spdlog::debug("Creating projector for Boolean select");
      return new BooleanProjector(subquery_results);
    }
    default: {
      spdlog::error("Invalid select type encountered.");
      throw std::runtime_error("Invalid select type encountered.");
    }
  }
}

void ResultProjector::join() {
  if (std::any_of(subquery_results_.begin(), subquery_results_.end(),
                  [](SubqueryResult subquery_result) {
                    return subquery_result.IsEmpty();
                  })) {
    spdlog::debug("Empty table encountered, terminating JOIN operation.");
    this->joined_results_ = this->getEmptyFinalTable();
  }
  SubqueryResult intermediate_result = SubqueryResult::FullNoSynonym();
  for (auto result : subquery_results_) {
    intermediate_result = intermediate_result.Join(result);
  }
  this->joined_results_ = intermediate_result;
}

SelectProjector::SelectProjector(std::vector<ElemReference *> &declarations, std::vector<SubqueryResult> &subquery_results)
    : ResultProjector(subquery_results), called_declarations_(declarations) {
  this->called_synonyms_.reserve(called_declarations_.size());
  for (auto *elem_ref : called_declarations_) {
    this->called_synonyms_.push_back(elem_ref->getSynonym());
  }
};

SubqueryResult SelectProjector::getEmptyFinalTable() {
  spdlog::debug("Creating empty table with all synonyms.");
  return SubqueryResult::Empty(this->called_synonyms_);
}

SubqueryResult SelectProjector::select_results() {
  for (auto *decl : called_declarations_) {
    auto *synonym = decl->getSynonym();
    if (!this->joined_results_.Uses(synonym)) {
      this->joined_results_ = this->joined_results_.AddColumn(synonym, decl->getContext());
    }
  }
  return this->joined_results_.GetColumns(this->called_synonyms_);
}

Result *SelectProjector::getResult() {
  this->join();
  SubqueryResult final_table = this->select_results();
  spdlog::debug("Element Result context size: {}", final_table.GetRows().size());
  return new ElemResult(this->called_declarations_, final_table);
}

SubqueryResult BooleanProjector::getEmptyFinalTable() {
  spdlog::debug("Creating empty table with no synonyms.");
  return SubqueryResult::FullNoSynonym();
}

bool BooleanProjector::has_results() {
  return !this->joined_results_.IsEmpty();
}

Result *BooleanProjector::getResult() {
  this->join();
  bool has_results = this->has_results();
  spdlog::debug("Boolean Result non-empty? {}", has_results);
  return new BooleanResult(has_results);
}