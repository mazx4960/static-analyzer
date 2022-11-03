#include "result_projector.h"
#include "spdlog/spdlog.h"
#include "qps/exceptions.h"

ResultProjector *ResultProjector::NewProjector(SelectCall *select_call, Database *database) {
  switch (select_call->GetSelectType()) {
    case SelectType::kElem: {
      spdlog::debug("Creating projector for Elem select");
      auto *elem_select_call = static_cast<ElemSelect *>(select_call);
      std::vector<ElemReference *> called_references = elem_select_call->GetReferences();
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
  spdlog::debug("Element Result table size: {}", final_table->Size());
  auto end = std::chrono::steady_clock::now();
  spdlog::info("Time taken to compute cross products: {} ms",
               std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count());

  for (auto *synonym : selected_synonyms) {
    if (!final_table->Uses(synonym)) {
      throw ResultCreationError("Synonym " + synonym->GetName() + " not found in table.");
    }
  }
  std::unordered_set<std::string> results_set;
  std::vector<ResultRow> rows = final_table->GetRows();
  results_set.reserve(rows.size());
  for (auto &row : rows) {
    std::string row_string;
    for (int i = 0; i < selected_.size(); i++) {
      auto *elem_ref = this->selected_[i];
      auto *synonym = elem_ref->getSynonym();
      auto *entity = row[synonym];

      if (i > 0) {
        row_string += " ";
      }
      if (elem_ref->getRefType() == ReferenceType::kAttr) {
        row_string += static_cast<AttrReference *>(elem_ref)->getAttribute(entity);
      } else {
        row_string += entity->GetValue();
      }
    }
    results_set.insert(row_string);
  }

  return new Result(selected_, results_set);
}

Result *BooleanSelectProjector::Project() {
  auto has_results = this->database_->HasResults();
  return has_results ? Result::True() : Result::False();
}