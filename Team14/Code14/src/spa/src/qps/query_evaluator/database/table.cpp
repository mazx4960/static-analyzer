// Copyright 2022 CS3203 Team14. All rights reserved.

#include "table.h"

#include "spdlog/spdlog.h"

Table::Table(const EntityPointerUnorderedMap &table, QueryReference *first, QueryReference *second) {
  if (first->getRefType() == ReferenceType::kSynonym || first->getRefType() == ReferenceType::kAttr) {
    QuerySynonym *first_synonym = static_cast<ElemReference *>(first)->getSynonym();
    synonyms_.push_back(first_synonym);
    if (second->getRefType() == ReferenceType::kSynonym || second->getRefType() == ReferenceType::kAttr) {
      QuerySynonym *second_synonym = static_cast<ElemReference *>(second)->getSynonym();
      // Corner case: first and second synonyms are the same
      if (*first_synonym == *second_synonym) {
        for (auto [entity, entity_set] : table) {
          // Only add entries with same first and second entity
          if (entity_set.find(entity) != entity_set.end()) {
            table_rows_.push_back(ResultRow({{first_synonym, entity}}));
          }
        }
      } else {
        synonyms_.push_back(second_synonym);
        for (auto [entity, entity_set] : table) {
          for (auto *other_entity : entity_set) {
            table_rows_.push_back(ResultRow{{first_synonym, entity}, {second_synonym, other_entity}});
          }
        }
      }
    } else {
      for (auto [entity, entity_set] : table) {
        if (!entity_set.empty()) {
          table_rows_.push_back(ResultRow{{first_synonym, entity}});
        }
      }
    }
  } else if (second->getRefType() == ReferenceType::kSynonym || second->getRefType() == ReferenceType::kAttr) {
    QuerySynonym *second_synonym = static_cast<ElemReference *>(second)->getSynonym();
    synonyms_.push_back(second_synonym);
    for (auto [entity, entity_set] : table) {
      for (auto *other_entity : entity_set) {
        table_rows_.push_back(ResultRow{{second_synonym, other_entity}});
      }
    }
  }
    // In this case there are no synonyms, but still need to add an entry if there are entries in the table
  else {
    for (auto [entity, entity_set] : table) {
      for (auto *other_entity : entity_set) {
        // Adds an empty hash table
        table_rows_.emplace_back();
        // Short circuit break since table will not need to add more elements
        return;
      }
    }
  }
}

Table::Table(std::vector<QuerySynonym *> synonyms, std::vector<ResultRow> result_rows)
    : synonyms_(std::move(synonyms)), table_rows_(std::move(result_rows)) {
}

bool Table::IsEmpty() {
  return table_rows_.empty();
}

bool Table::Uses(QuerySynonym *synonym) const {
  // Need to use find_if for pointer comparison
  return std::find_if(synonyms_.begin(), synonyms_.end(), [synonym](QuerySynonym *searched_synonym) {
    return *synonym == *searched_synonym;
  }) != synonyms_.end();
}

std::vector<QuerySynonym *> Table::GetCommonSynonyms(Table *other) {
  std::vector<QuerySynonym *> common_synonyms{};
  for (auto *synonym : synonyms_) {
    if (other->Uses(synonym)) {
      common_synonyms.push_back(synonym);
    }
  }
  return common_synonyms;
}

EntityPointerUnorderedSet Table::GetColumn(QuerySynonym *synonym) {
  if (std::find(synonyms_.begin(), synonyms_.end(), synonym) == synonyms_.end()) {
    return EntityPointerUnorderedSet{};
  }
  EntityPointerUnorderedSet results{};
  results.reserve(table_rows_.size());
  for (auto row : table_rows_) {
    results.insert(row[synonym]);
  }
  return results;
}

Table *Table::GetColumns(const std::vector<QuerySynonym *> &synonyms) {
  std::vector<QuerySynonym *> new_synonyms{};
  for (auto *synonym : synonyms) {
    if (std::find(synonyms_.begin(), synonyms_.end(), synonym) != synonyms_.end()) {
      new_synonyms.push_back(synonym);
    }
  }
  if (new_synonyms.empty()) {
    return FullNoSynonym();
  }
  std::vector<ResultRow> new_rows{};
  for (auto row : table_rows_) {
    ResultRow new_row{};
    for (auto *synonym : new_synonyms) {
      new_row[synonym] = row[synonym];
    }
    new_rows.push_back(new_row);
  }
  return new Table(new_synonyms, new_rows);
}

Table *Table::Join(Table *other) {
  auto common_synonyms = this->GetCommonSynonyms(other);
  std::vector<QuerySynonym *> all_synonyms{};
  all_synonyms.reserve(synonyms_.size() + other->synonyms_.size());
  all_synonyms.insert(all_synonyms.end(), synonyms_.begin(), synonyms_.end());
  all_synonyms.insert(all_synonyms.end(), other->synonyms_.begin(), other->synonyms_.end());
  std::sort(all_synonyms.begin(), all_synonyms.end(), QuerySynonymPointerComparison());
  all_synonyms.erase(std::unique(all_synonyms.begin(), all_synonyms.end(), QuerySynonymPointerEquality()),
                     all_synonyms.end());

  std::vector<std::vector<ResultRow>::iterator> first_copy;
  first_copy.reserve(this->table_rows_.size());
  for (int i = 0; i < this->table_rows_.size(); ++i) {
    first_copy.push_back(this->table_rows_.begin() + i);
  }
  std::vector<std::vector<ResultRow>::iterator> second_copy;
  first_copy.reserve(other->table_rows_.size());
  second_copy.reserve(other->table_rows_.size());
  for (int i = 0; i < other->table_rows_.size(); ++i) {
    second_copy.push_back(other->table_rows_.begin() + i);
  }
  auto cmp =
      [common_synonyms](const std::vector<ResultRow>::iterator &first, const std::vector<ResultRow>::iterator &second) {
        for (auto *synonym : common_synonyms) {
          if ((*first)[synonym]->GetValue() != (*second)[synonym]->GetValue()) {
            return (*first)[synonym]->GetValue() < (*second)[synonym]->GetValue();
          }
        }
        return false;
      };
  auto eq =
      [common_synonyms](const std::vector<ResultRow>::iterator &first, const std::vector<ResultRow>::iterator &second) {
        return std::all_of(common_synonyms.begin(), common_synonyms.end(), [first, second](QuerySynonym *synonym) {
          return (*first)[synonym]->GetValue() == (*second)[synonym]->GetValue();
        });
      };
  // Sort rows by values in common columns
  std::sort(first_copy.begin(), first_copy.end(), cmp);
  std::sort(second_copy.begin(), second_copy.end(), cmp);

  // Get intersection via merge sort method

  std::vector<ResultRow> new_rows{};
  auto first_iter = first_copy.begin();
  auto second_iter = second_copy.begin();
  while (first_iter != first_copy.end() && second_iter != second_copy.end()) {
    if (eq(*first_iter, *second_iter)) {
      auto first_end = first_iter;
      while (first_end != first_copy.end() && eq(*first_iter, *first_end)) {
        ++first_end;
      }
      auto second_end = second_iter;
      while (second_end != second_copy.end() && eq(*second_iter, *second_end)) {
        ++second_end;
      }
      for (auto first_common = first_iter; first_common != first_end; ++first_common) {
        for (auto second_common = second_iter; second_common != second_end; ++second_common) {
          // Merge rows
          ResultRow new_row = **first_common;
          new_row.insert((*second_common)->begin(), (*second_common)->end());
          new_rows.push_back(new_row);
        }
      }
      first_iter = first_end;
      second_iter = second_end;
    } else if (cmp(*first_iter, *second_iter)) {
      ++first_iter;
    } else {
      ++second_iter;
    }
  }
  spdlog::debug("Number of rows in result: {}", new_rows.size());
  return new Table(all_synonyms, new_rows);
}
Table *Table::Empty(std::vector<QuerySynonym *> synonyms) {
  return new Table(std::move(synonyms), {});
}

Table *Table::FullNoSynonym() {
  return new Table({}, {{}});
}
Table *Table::AddColumn(QuerySynonym *synonym, const EntityPointerUnorderedSet &entities) {
  std::vector<ResultRow> new_rows{};
  for (const auto &row : table_rows_) {
    for (auto *entity : entities) {
      ResultRow new_row = row;
      new_row[synonym] = entity;
      new_rows.push_back(new_row);
    }
  }
  std::vector<QuerySynonym *> new_synonyms = synonyms_;
  new_synonyms.push_back(synonym);
  return new Table(new_synonyms, new_rows);
}
const std::vector<ResultRow> &Table::GetRows() const {
  return table_rows_;
}
int Table::Size() const {
  return this->table_rows_.size();
}
std::string Table::ToString() {
  std::string synonym_string;
  for (auto *synonym : synonyms_) {
    synonym_string += synonym->ToString() + " ";
  }
  return fmt::format("Table with {} rows, synonyms: <{}>", table_rows_.size(), synonym_string);
}
