#include "result_projector.h"
#include "spdlog/spdlog.h"

ResultProjector *ResultProjector::getProjector(SelectCall *select_call, std::vector<SubqueryResult> &subquery_results) {
  switch (select_call->getSelectType()) {
    case SelectType::kElem: {
      auto *elem_select_call = static_cast<ElemSelect *>(select_call);
      std::vector<ElemReference *> called_references = elem_select_call->getReferences();
      return new SelectProjector(called_references, subquery_results);
    }
    case SelectType::kBoolean: {
      return new BooleanProjector(subquery_results);
    }
    default: throw std::runtime_error("Invalid select type.");
  }
}

void ResultProjector::join() {
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

void SelectProjector::project() {
  if (std::any_of(subquery_results_.begin(), subquery_results_.end(),
                  [](SubqueryResult subquery_result) {
                    return subquery_result.IsEmpty();
                  })) {
    spdlog::debug("Some table is empty");
    this->joined_results_ = SubqueryResult::Empty(this->called_synonyms_);
  } else {
    this->join();
  }
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
  this->project();
  SubqueryResult final_table = this->select_results();
  return new ElemResult(this->called_declarations_, final_table);
}

bool BooleanProjector::has_results() {
  return !this->joined_results_.IsEmpty();
}

Result *BooleanProjector::getResult() {
  this->join();
  return new BooleanResult(this->has_results());
}