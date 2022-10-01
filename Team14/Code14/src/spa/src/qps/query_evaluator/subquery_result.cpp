//R
// Created by gabri on 19/9/2022.
//
#include "subquery_result.h"

#include <utility>

#include "spdlog/spdlog.h"

SubqueryResult::SubqueryResult(const EntityPointerUnorderedMap &table, QueryDeclaration *first, QueryDeclaration *second)
    : table_(table), first_decl_(first), second_decl_(second) {
  QuerySynonym* first_synonym = first->getSynonym();
  QuerySynonym* second_synonym = second->getSynonym();
  if (first_synonym != QuerySynonym::empty()) {
    synonyms_.push_back(first_synonym);
    if (second->getSynonym() != QuerySynonym::empty()) {
      synonyms_.push_back(second_synonym);
      for (auto [entity, entity_set] : table) {
        for (auto *other_entity : entity_set) {
          table_rows_.push_back(ResultRow{{first_synonym, entity}, {second_synonym, other_entity}});
        }
      }
    }
    else {
      for (auto [entity, entity_set] : table) {
        table_rows_.push_back(ResultRow{{first_synonym, entity}});
      }
    }
  }
  else if (second->getSynonym() != QuerySynonym::empty()) {
    synonyms_.push_back(second_synonym);
    for (auto [entity, entity_set] : table) {
      for (auto *other_entity : entity_set) {
        table_rows_.push_back(ResultRow{{second_synonym, other_entity}});
      }
    }
  }
}

SubqueryResult::SubqueryResult(EntityPointerUnorderedMap table, EntityPointerUnorderedMap table_inv,
                               QueryDeclaration *first, QueryDeclaration *second)
    : table_(std::move(table)), table_inv_(std::move(table_inv)), first_decl_(first), second_decl_(second) {
}

SubqueryResult::SubqueryResult(std::vector<QuerySynonym *> synonyms, std::vector<ResultRow> result_rows)
    : synonyms_(std::move(synonyms)), table_rows_(std::move(result_rows)) {}

bool SubqueryResult::empty() {
  return table_.empty() || table_inv_.empty();
}

SubqueryResult SubqueryResult::invert() {
  return SubqueryResult(table_inv_, table_, second_decl_, first_decl_);
}

bool SubqueryResult::uses(QueryDeclaration *decl) {
  return (decl->getType() == first_decl_->getType() && decl->getSynonym() == first_decl_->getSynonym())
      || (decl->getType() == second_decl_->getType() && decl->getSynonym() == second_decl_->getSynonym());
}

std::vector<QuerySynonym *> SubqueryResult::getCommonSynonyms(const SubqueryResult &other) {
  std::vector<QuerySynonym *> common_synonyms(std::min(synonyms_.size(), other.synonyms_.size()));
  auto end_pos = std::set_intersection(synonyms_.begin(), synonyms_.end(), other.synonyms_.begin(), other.synonyms_.end(), common_synonyms.begin());
  return std::vector<QuerySynonym *>(common_synonyms.begin(), end_pos);
}
EntityPointerUnorderedSet SubqueryResult::GetColumn(QuerySynonym *synonym) {
  if (std::find(synonyms_.begin(), synonyms_.end(), synonym) != synonyms_.end()) {
    return EntityPointerUnorderedSet{};
  }
  EntityPointerUnorderedSet results{};
  results.reserve(table_rows_.size());
  for (auto row : table_rows_) {
    results.insert(row[synonym]);
  }
  return results;
}

SubqueryResult SubqueryResult::Intersect(SubqueryResult &other) {
  if (this->getCommonSynonyms(other).size() != 2) {
    return SubqueryResult(EntityPointerUnorderedMap(), first_decl_, second_decl_);
  }
  if (*this->first_decl_ == *other.second_decl_) {
    return this->invert().Intersect(other);
  }
  spdlog::debug("first: {}", first_decl_->getSynonym()->toString());
  spdlog::debug("second: {}", other.first_decl_->getSynonym()->toString());
  EntityPointerUnorderedMap intersection{};
  spdlog::debug("sizes: {}, {}", table_.size(), other.table_.size());
  for (auto [key, values] : table_) {
    spdlog::debug("First: {}", key->ToString());
    if (other.table_.find(key) != other.table_.end()) {
      spdlog::debug("Adding key for entity: {}", key->ToString());
      intersection[key] = EntityPointerUnorderedSet{};
      for (auto *value : values) {
        if (other.table_[key].find(value) != other.table_[key].end()) {
          spdlog::debug("Adding entity: {}", value->ToString());
          intersection[key].insert(value);
        }
      }
    }
  }
  return SubqueryResult(intersection, first_decl_, second_decl_);
}

SubqueryResult SubqueryResult::Join(SubqueryResult &other) {
  auto common_synonyms = this->getCommonSynonyms(other);
  std::vector<QuerySynonym *> all_synonyms{};
  all_synonyms.reserve(synonyms_.size() + other.synonyms_.size());
  all_synonyms.insert(all_synonyms.end(), synonyms_.begin(), synonyms_.end());
  all_synonyms.insert(all_synonyms.end(), other.synonyms_.begin(), other.synonyms_.end());
  std::sort(all_synonyms.begin(), all_synonyms.end());
  all_synonyms.erase(std::unique(all_synonyms.begin(), all_synonyms.end()), all_synonyms.end());

  std::vector<ResultRow > new_rows{};
  for (auto this_row : table_rows_) {
    for (auto that_row : other.table_rows_) {
      bool can_join = true;
      for (auto *syn : common_synonyms) {
        if (this_row[syn] != that_row[syn]) {
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
  return SubqueryResult(all_synonyms, new_rows);
}
