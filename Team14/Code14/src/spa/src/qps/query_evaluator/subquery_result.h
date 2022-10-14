//
// Created by gabri on 19/9/2022.
//
#pragma once

#include "pkb/relationship/relationship_table.h"
#include "qps/pql/query_reference.h"
using ResultRow = std::unordered_map<QuerySynonym *,
                                     Entity *,
                                     QuerySynonymHashFunction,
                                     QuerySynonymPointerEquality>;

class SubqueryResult {
 private:

  std::vector<QuerySynonym *> synonyms_;
  std::vector<ResultRow> table_rows_;

  SubqueryResult(std::vector<QuerySynonym *>, std::vector<ResultRow>);
 public:
  SubqueryResult(const EntityPointerUnorderedMap &, QueryReference *, QueryReference *);
  [[nodiscard]] bool IsEmpty();
  bool Uses(QuerySynonym *synonym);
  std::vector<QuerySynonym *> GetCommonSynonyms(SubqueryResult other);
  EntityPointerUnorderedSet GetColumn(QuerySynonym *);
  SubqueryResult Join(SubqueryResult &other);
  SubqueryResult GetColumns(const std::vector<QuerySynonym *>& synonyms);
  SubqueryResult AddColumn(QuerySynonym *, EntityPointerUnorderedSet);
  static SubqueryResult Empty(std::vector<QuerySynonym *> synonyms);
  static SubqueryResult FullNoSynonym();
  [[nodiscard]] const std::vector<ResultRow> &GetRows() const;
};
