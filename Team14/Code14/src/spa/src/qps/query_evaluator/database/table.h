// Copyright 2022 CS3203 Team14. All rights reserved.

#pragma once

#include <utility>
#include <vector>
#include <unordered_map>

#include "pkb/relationship/relationship_table.h"
#include "qps/pql/query_reference.h"

using ResultRow = std::unordered_map<QuerySynonym *, Entity *, QuerySynonymHashFunction, QuerySynonymPointerEquality>;

class Table {
 private:

  std::vector<QuerySynonym *> synonyms_;

  std::vector<ResultRow> table_rows_;

 public:
  Table(std::vector<QuerySynonym *>, std::vector<ResultRow>);
  Table(const EntityPointerUnorderedMap &, QueryReference *, QueryReference *);
  [[nodiscard]] bool IsEmpty();
  bool Uses(QuerySynonym *synonym) const;
  std::vector<QuerySynonym *> GetCommonSynonyms(Table *other);
  EntityPointerUnorderedSet GetColumn(QuerySynonym *);
  Table *Join(Table *other);
  Table *GetColumns(const std::vector<QuerySynonym *> &synonyms);
  Table *AddColumn(QuerySynonym *, const EntityPointerUnorderedSet &);
  static Table *Empty(std::vector<QuerySynonym *> synonyms);
  static Table *FullNoSynonym();
  [[nodiscard]] const std::vector<ResultRow> &GetRows() const;
  [[nodiscard]] int Size() const;
  std::string ToString();
};
