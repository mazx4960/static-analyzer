//
// Created by gabri on 19/9/2022.
//
#pragma once

#include "pkb/relationship/relationship_table.h"
#include "qps/pql/query_declaration.h"
using EntityPointerUnorderedMap = std::unordered_map<Entity *,
                                                     EntityPointerUnorderedSet,
                                                     EntityHashFunction,
                                                     EntityPointerEquality>;
using ResultRow = std::unordered_map<QuerySynonym *,
                                     Entity *,
                                     QuerySynonymHashFunction,
                                     QuerySynonymPointerEquality>;

class SubqueryResult {
 private:
  EntityPointerUnorderedMap table_;

  EntityPointerUnorderedMap table_inv_;

  QueryDeclaration *first_decl_;

  QueryDeclaration *second_decl_;

  std::vector<QuerySynonym *> synonyms_;
  std::vector<ResultRow > table_rows_;

  SubqueryResult(EntityPointerUnorderedMap, EntityPointerUnorderedMap, QueryDeclaration *, QueryDeclaration *);
 public:
  SubqueryResult(const EntityPointerUnorderedMap &, QueryDeclaration *, QueryDeclaration *);
  [[nodiscard]] bool empty();
  SubqueryResult invert();
  bool uses(QueryDeclaration *decl);
  std::vector<QueryDeclaration *> getCommonSynonyms(const SubqueryResult &other);
  EntityPointerUnorderedSet GetColumn(QuerySynonym *);
  SubqueryResult Intersect(SubqueryResult &other);
  SubqueryResult Join(SubqueryResult &other);
};
