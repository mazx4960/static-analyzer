// Copyright 2022 CS3203 Team14. All rights reserved.

#pragma once

#include "pkb/relationship/relationship_table.h"
#include "qps/pql/query_reference.h"

using ResultRow = std::unordered_map<QuerySynonym *, Entity *, QuerySynonymHashFunction, QuerySynonymPointerEquality>;

class SubqueryResult {
 private:

  std::vector<QuerySynonym *> synonyms_;

  std::vector<ResultRow> table_rows_;

 public:
  SubqueryResult(std::vector<QuerySynonym *>, std::vector<ResultRow>);
  SubqueryResult(const EntityPointerUnorderedMap &, QueryReference *, QueryReference *);
  [[nodiscard]] bool IsEmpty();
  bool Uses(QuerySynonym *synonym);
  std::vector<QuerySynonym *> GetCommonSynonyms(SubqueryResult other);
  EntityPointerUnorderedSet GetColumn(QuerySynonym *);
  SubqueryResult Join(SubqueryResult &other);
  SubqueryResult GetColumns(const std::vector<QuerySynonym *> &synonyms);
  SubqueryResult AddColumn(QuerySynonym *, const EntityPointerUnorderedSet &);
  static SubqueryResult Empty(std::vector<QuerySynonym *> synonyms);
  static SubqueryResult FullNoSynonym();
  [[nodiscard]] const std::vector<ResultRow> &GetRows() const;
  [[nodiscard]] int Size() const;
};
