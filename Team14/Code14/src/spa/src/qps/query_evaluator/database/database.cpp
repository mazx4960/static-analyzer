// Copyright 2022 CS3203 Team14. All rights reserved.

#include <spdlog/spdlog.h>

#include "database.h"

std::vector<Table *> Database::Empty(std::vector<QuerySynonym *> synonyms) {
  return {Table::Empty(std::move(synonyms))};
}
void Database::AddTable(Table *table) {
  unmerged_tables_.push_back(table);
}

void Database::OrderTables() {
  std::sort(unmerged_tables_.begin(), unmerged_tables_.end(), [](Table *first, Table *second) -> bool {
    return first->Size() < second->Size();
  });
}

void Database::MergeTables() {
  if (IsEmpty()) {
    spdlog::debug("Empty table encountered, terminating JOIN operation.");
    std::vector<QuerySynonym *> synonyms;
    std::copy(all_synonyms_.begin(), all_synonyms_.end(), std::back_inserter(synonyms));
    this->merged_tables_ = this->Empty(synonyms);
    return;
  }
  while (!unmerged_tables_.empty()) {
    Table *table = unmerged_tables_.back();
    unmerged_tables_.pop_back();

    std::vector<std::vector<Table *>::iterator> matches{};
    Table *merged_table = table;
    for (auto irt = merged_tables_.begin(); irt != merged_tables_.end(); ++irt) {
      if (!table->GetCommonSynonyms(*irt).empty()) {
        matches.push_back(irt);
        merged_table = merged_table->Join(*irt);
      }
    }
    for (auto match = matches.rbegin(); match != matches.rend(); ++match) {
      merged_tables_.erase(*match);
    }
    merged_tables_.push_back(merged_table);
  }
  spdlog::debug("Merged tables: {}", merged_tables_.size());
  for (auto *table : merged_tables_) {
    spdlog::debug("Table: {}", table->ToString());
  }
}
bool Database::IsEmpty() const {
  return std::any_of(unmerged_tables_.begin(), unmerged_tables_.end(), [](Table *table) {
    return table->IsEmpty();
  });
}
bool Database::HasResults() const {
  return merged_tables_.empty() || !std::any_of(merged_tables_.begin(), merged_tables_.end(), [](Table *table) {
    return table->IsEmpty();
  });
}
Table *Database::GetTable(const std::vector<QuerySynonym *> &synonyms) {
  Table *final_table = Table::FullNoSynonym();
  for (Table *table : merged_tables_) {
    auto *columns = table->GetColumns(synonyms);
    final_table = final_table->Join(columns);
  }
  // Get the remaining synonyms via context
  for (auto *synonym : synonyms) {
    if (!final_table->Uses(synonym)) {
      final_table = final_table->AddColumn(synonym, ctx_->Get(synonym));
    }
  }
  return final_table;
}