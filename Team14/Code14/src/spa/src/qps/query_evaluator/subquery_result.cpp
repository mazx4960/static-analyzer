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