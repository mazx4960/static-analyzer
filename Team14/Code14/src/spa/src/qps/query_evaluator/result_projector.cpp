#include "result_projector.h"
#include "spdlog/spdlog.h"

ResultProjector *ResultProjector::NewProjector(SelectCall *select_call, Database *database) {
  switch (select_call->getSelectType()) {
    case SelectType::kElem: {
      spdlog::debug("Creating projector for Elem select");
      auto *elem_select_call = static_cast<ElemSelect *>(select_call);
      std::vector<ElemReference *> called_references = elem_select_call->getReferences();
      return new ElemSelectProjector(called_references, database);
    }
    case SelectType::kBoolean: {
      spdlog::debug("Creating projector for Boolean select");
      return new BooleanSelectProjector(database);
    }
    default: {
      spdlog::error("Invalid select type encountered.");
      throw std::runtime_error("Invalid select type encountered.");
    }
  }
}

Result *ElemSelectProjector::Project() {
  auto begin = std::chrono::steady_clock::now();
  std::vector<QuerySynonym *> selected_synonyms;
  for (auto *elem_ref : this->selected_) {
    selected_synonyms.push_back(elem_ref->getSynonym());
  }
  Table *final_table = this->database_->GetTable(selected_synonyms);
  spdlog::debug("Element Result context size: {}", final_table->GetRows().size());
  auto end = std::chrono::steady_clock::now();
  spdlog::info("Time taken to compute cross products: {} ms",
               std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count());
  return new ElemResult(selected_, final_table);
}

Result *BooleanSelectProjector::Project() {
  auto has_results = this->database_->HasResults();
  return new BooleanResult(has_results);
}