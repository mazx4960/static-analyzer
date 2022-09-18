//
// Created by gabri on 19/9/2022.
//
#include "subquery_result.h"

SubqueryResult::SubqueryResult(EntityPointerUnorderedMap table, QueryDeclaration* first, QueryDeclaration* second)
    : table_(table), table_inv_(), first_decl_(first), second_decl_(second) {
  for (auto [entity, entity_set] : table) {
    for (auto other_entity : entity_set) {
      if (table_inv_.find(other_entity) == table_inv_.end()) {
        table_inv_[other_entity] = EntityPointerUnorderedSet();
      }
      table_inv_[other_entity].insert(entity);
    }
  }
}

SubqueryResult::SubqueryResult(EntityPointerUnorderedMap table, EntityPointerUnorderedMap table_inv,
                               QueryDeclaration* first, QueryDeclaration* second)
    : table_(table), table_inv_(table_inv), first_decl_(first), second_decl_(second) {}

bool SubqueryResult::empty() {
  return table_.empty() || table_inv_.empty();
}

SubqueryResult SubqueryResult::invert() {
  return SubqueryResult(table_inv_, table_, second_decl_, first_decl_);
}

bool SubqueryResult::uses(QueryDeclaration* decl) {
  return *decl == *first_decl_ || *decl == *second_decl_;
}

std::vector<QueryDeclaration *> SubqueryResult::getCommonSynonyms(SubqueryResult other) {
  std::vector<QueryDeclaration *> common_synonyms;
  if (first_decl_->getSynonym() != QuerySynonym::empty() &&
      (*first_decl_ == *other.first_decl_ || *first_decl_ == *other.second_decl_)) {
    common_synonyms.push_back(first_decl_);
  }
  if (second_decl_->getSynonym() != QuerySynonym::empty() &&
      (*second_decl_ == *other.first_decl_ || *second_decl_ == *other.second_decl_)) {
    common_synonyms.push_back(second_decl_);
  }
  return common_synonyms;
}
EntityPointerUnorderedSet SubqueryResult::GetColumn(QuerySynonym* synonym) {
  if (*first_decl_->getSynonym() == *synonym) {
    EntityPointerUnorderedSet entities{};
    for (auto [key, values] : table_) {
      entities.insert(key);
    }
    return entities;
  }
  if (*second_decl_->getSynonym() == *synonym) {
    EntityPointerUnorderedSet entities{};
    for (auto [key, values] : table_inv_) {
      entities.insert(key);
    }
    return entities;
  }
  return EntityPointerUnorderedSet();
}

SubqueryResult SubqueryResult::Intersect(SubqueryResult other) {
  if (this->getCommonSynonyms(other).size() != 2) {
    return SubqueryResult(EntityPointerUnorderedMap(), first_decl_, second_decl_);
  }
  if (this->first_decl_ == other.second_decl_) {
    return other.Intersect(*this);
  }
  EntityPointerUnorderedMap intersection{};
  for (auto [key, values] : table_) {
    intersection[key] = EntityPointerUnorderedSet{};
    for (auto value : values) {
      if (other.table_.find(key) != other.table_.end()
          && other.table_[key].find(value) != other.table_[key].end()) {
        intersection[key].insert(value);
      }
    }
  }
  return SubqueryResult(intersection, first_decl_, second_decl_);
}

SubqueryResult SubqueryResult::Join(SubqueryResult other) {
  auto common_synonyms = this->getCommonSynonyms(other);
  if (common_synonyms.size() != 1) {
    return SubqueryResult(EntityPointerUnorderedMap(), first_decl_, second_decl_);
  }
  auto common_synonym = common_synonyms[0];
  QueryDeclaration* first = (*common_synonym == *first_decl_) ? second_decl_ : first_decl_;
  QueryDeclaration* third = (*common_synonym == *other.first_decl_) ? other.second_decl_ : other.first_decl_;
  EntityPointerUnorderedMap& first_table = (*common_synonym == *first_decl_) ? table_ : table_inv_;
  EntityPointerUnorderedMap& second_table = (*common_synonym == *other.first_decl_) ? other.table_inv_ : other.table_inv_;
  EntityPointerUnorderedMap join{};
  for (auto [key, values] : first_table) {
    join[key] = EntityPointerUnorderedSet{};
    for (auto value_key : values) {
      if (second_table.find(value_key) != second_table.end()) {
        for (auto value : second_table[value_key]) { join[key].insert(value); }
      }
    }
  }
  return SubqueryResult(join, first, third);
}
