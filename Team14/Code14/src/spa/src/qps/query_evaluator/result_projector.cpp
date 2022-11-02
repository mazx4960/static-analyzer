#include "result_projector.h"
#include "spdlog/spdlog.h"

ResultProjector *ResultProjector::getProjector(SelectCall *select_call, std::vector<SubqueryResult> &subquery_results) {
  switch (select_call->getSelectType()) {
    case SelectType::kElem: {
      spdlog::debug("Creating projector for Elem select");
      auto *elem_select_call = static_cast<ElemSelect *>(select_call);
      std::vector<ElemReference *> called_references = elem_select_call->getReferences();
      return new ElemSelectProjector(called_references, subquery_results);
    }
    case SelectType::kBoolean: {
      spdlog::debug("Creating projector for Boolean select");
      return new BooleanSelectProjector(subquery_results);
    }
    default: {
      spdlog::error("Invalid select type encountered.");
      throw std::runtime_error("Invalid select type encountered.");
    }
  }
}

void ResultProjector::join() {
  if (std::any_of(subquery_results_.begin(), subquery_results_.end(), [](SubqueryResult subquery_result) {
    return subquery_result.IsEmpty();
  })) {
    spdlog::debug("Empty table encountered, terminating JOIN operation.");
    this->joined_results_ = this->getEmptyFinalTable();
    return;
  }
  std::vector<SubqueryResult> intermediate_results{};
  for (auto result : subquery_results_) {
    std::vector<std::vector<SubqueryResult>::iterator> matches{};
    SubqueryResult merged_result = result;
    for (auto irt = intermediate_results.begin(); irt != intermediate_results.end(); ++irt) {
      if (!result.GetCommonSynonyms(*irt).empty()) {
        matches.push_back(irt);
        merged_result = merged_result.Join(*irt);
      }
    }
    for (auto match = matches.rbegin(); match != matches.rend(); ++match) {
      intermediate_results.erase(*match);
    }
    intermediate_results.push_back(merged_result);
  }
  for (auto irt : intermediate_results) {
    if (irt.IsEmpty()) {
      // Table must be empty with no results
      joined_results_ = {SubqueryResult::Empty({})};
      // Short circuit the evaluation
      return;
    }
  }
  joined_results_ = intermediate_results;
}

ElemSelectProjector::ElemSelectProjector(std::vector<ElemReference *> &declarations,
                                         std::vector<SubqueryResult> &subquery_results) : ResultProjector(
    subquery_results), called_declarations_(declarations) {
  this->called_synonyms_.reserve(called_declarations_.size());
  for (auto *elem_ref : called_declarations_) {
    this->called_synonyms_.push_back(elem_ref->getSynonym());
  }
};

std::vector<SubqueryResult> ElemSelectProjector::getEmptyFinalTable() {
  spdlog::debug("Creating empty table with all synonyms.");
  return {SubqueryResult::Empty(this->called_synonyms_)};
}

SubqueryResult ElemSelectProjector::selectResults(Context *ctx) {
  // Take the required synonyms from the existing tables via cross product
  SubqueryResult joined_result = SubqueryResult::FullNoSynonym();
  for (SubqueryResult &result : joined_results_) {
    auto filtered_results = result.GetColumns(called_synonyms_);
    joined_result = joined_result.Join(filtered_results);
  }
  // Get the remaining synonyms via context
  for (auto *decl : called_declarations_) {
    auto *synonym = decl->getSynonym();
    if (!joined_result.Uses(synonym)) {
      joined_result = joined_result.AddColumn(synonym, ctx->Get(synonym));
    }
  }
  return joined_result.GetColumns(this->called_synonyms_);
}

Result *ElemSelectProjector::project(Context *ctx) {
  this->join();
  SubqueryResult final_table = this->selectResults(ctx);
  spdlog::debug("Element Result context size: {}", final_table.GetRows().size());
  return new ElemResult(this->called_declarations_, final_table);
}

std::vector<SubqueryResult> BooleanSelectProjector::getEmptyFinalTable() {
  spdlog::debug("Creating empty table with no synonyms.");
  return {SubqueryResult::Empty({})};
}

bool BooleanSelectProjector::has_results() {
  return this->joined_results_.empty() || !this->joined_results_[0].IsEmpty();
}

Result *BooleanSelectProjector::project(Context * /*ctx*/) {
  this->join();
  bool has_results = this->has_results();
  spdlog::debug("Boolean Result non-empty? {}", has_results);
  return new BooleanResult(has_results);
}