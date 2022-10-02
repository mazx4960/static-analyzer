//R
// Created by gabri on 19/9/2022.
//
#include "subquery_result.h"

#include <utility>

#include "spdlog/spdlog.h"

SubqueryResult::SubqueryResult(const EntityPointerUnorderedMap &table, QueryDeclaration *first, QueryDeclaration *second)
{
  QuerySynonym* first_synonym = first->getSynonym();
  QuerySynonym* second_synonym = second->getSynonym();
  if (*first_synonym != *QuerySynonym::empty()) {
    spdlog::debug("First synonym used");
    synonyms_.push_back(first_synonym);
    if (*second_synonym != *QuerySynonym::empty()) {
      spdlog::debug("Second synonym used");
      // Corner case: first and second synonyms are the same
      if (*first_synonym == *second_synonym) {
        for (auto [entity, entity_set] : table) {
          // Only add entries with same first and second entity
          if (entity_set.find(entity) != entity_set.end()) {
            table_rows_.push_back(ResultRow({{first_synonym, entity}}));
          }
        }
      }
      else {
        synonyms_.push_back(second_synonym);
        for (auto [entity, entity_set] : table) {
          for (auto *other_entity : entity_set) {
            table_rows_.push_back(ResultRow{{first_synonym, entity}, {second_synonym, other_entity}});
          }
        }
      }
    }
    else {
      for (auto [entity, entity_set] : table) {
        table_rows_.push_back(ResultRow{{first_synonym, entity}});
      }
    }
  }
  else if (*second_synonym != *QuerySynonym::empty()) {
    spdlog::debug("Second synonym used");
    synonyms_.push_back(second_synonym);
    for (auto [entity, entity_set] : table) {
      for (auto *other_entity : entity_set) {
        table_rows_.push_back(ResultRow{{second_synonym, other_entity}});
      }
    }
  }
  // In this case there are no synonyms, but still need to add an entry if there are entries in the table
  else {
    spdlog::debug("No synonyms used");
    for (auto [entity, entity_set] : table) {
      for (auto *other_entity : entity_set) {
        // Adds an empty hash table
        table_rows_.emplace_back();
        // Short circuit break since table will not need to add more elements
        return;
      }
    }
  }
  std::string synonym_string;
  for (auto *syn : synonyms_) synonym_string += syn->toString() + ", ";
  spdlog::debug("Creating new table with synonyms: {} and tuples:", synonym_string);
  for (auto row : table_rows_) {
    std::string row_string;
    for (auto *syn : synonyms_) row_string += row[syn]->ToString() + ", ";
    spdlog::debug("({})", row_string);
  }
}

SubqueryResult::SubqueryResult(std::vector<QuerySynonym *> synonyms, std::vector<ResultRow> result_rows)
    : synonyms_(std::move(synonyms)), table_rows_(std::move(result_rows)) {
  std::string synonym_string;
  for (auto *syn : synonyms_) synonym_string += syn->toString() + ", ";
  spdlog::debug("Creating new table with synonyms: {} and tuples:", synonym_string);
  for (auto row : table_rows_) {
    std::string row_string;
    for (auto *syn : synonyms_) row_string += row[syn]->ToString() + ", ";
    spdlog::debug("({})", row_string);
  }
}

bool SubqueryResult::IsEmpty() {
  return table_rows_.empty();
}

bool SubqueryResult::Uses(QuerySynonym *synonym) {
  // Need to use find_if for pointer comparison
  return std::find_if(synonyms_.begin(), synonyms_.end(), [synonym](QuerySynonym* searched_synonym){
           return *synonym == *searched_synonym;
         }) != synonyms_.end();
}

std::vector<QuerySynonym *> SubqueryResult::GetCommonSynonyms(const SubqueryResult &other) {
  std::vector<QuerySynonym *> common_synonyms(std::min(synonyms_.size(), other.synonyms_.size()));
  auto end_pos = std::set_intersection(synonyms_.begin(), synonyms_.end(), other.synonyms_.begin(), other.synonyms_.end(), common_synonyms.begin(), QuerySynonymPointerEquality());
  return std::vector<QuerySynonym *>(common_synonyms.begin(), end_pos);
}
EntityPointerUnorderedSet SubqueryResult::GetColumn(QuerySynonym *synonym) {
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

SubqueryResult SubqueryResult::Join(SubqueryResult &other) {
  auto common_synonyms = this->GetCommonSynonyms(other);
  std::vector<QuerySynonym *> all_synonyms{};
  all_synonyms.reserve(synonyms_.size() + other.synonyms_.size());
  all_synonyms.insert(all_synonyms.end(), synonyms_.begin(), synonyms_.end());
  all_synonyms.insert(all_synonyms.end(), other.synonyms_.begin(), other.synonyms_.end());
  std::sort(all_synonyms.begin(), all_synonyms.end());
  all_synonyms.erase(std::unique(all_synonyms.begin(), all_synonyms.end()), all_synonyms.end());

  std::vector<ResultRow > new_rows{};
  for (auto this_row : table_rows_) {
    std::string first_row_string;
    for (auto *syn : common_synonyms) first_row_string += this_row[syn]->ToString() + ", ";
    for (auto that_row : other.table_rows_) {
      std::string second_row_string;
      for (auto *syn : common_synonyms) second_row_string += that_row[syn]->ToString() + ", ";
      bool can_join = true;
      spdlog::debug("Comparing ({}) to ({})", first_row_string, second_row_string);
      for (auto *syn : common_synonyms) {
        if (!(*this_row[syn] == *that_row[syn])) {
          spdlog::debug("Fail at synonym {}", syn->toString());
          can_join = false;
          break;
        }
      }
      if (can_join) {
        // Creates a hashmap with the contents of the two hash maps
        ResultRow new_row = this_row;
        new_row.insert(that_row.begin(), that_row.end());
        new_rows.push_back(new_row);
      }
    }
  }
  spdlog::debug("Number of rows in result: {}", new_rows.size());
  return SubqueryResult(all_synonyms, new_rows);
}
