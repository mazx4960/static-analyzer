// Copyright 2022 CS3203 Team14. All rights reserved.

#pragma once

#include <random>
#include <utility>
#include <vector>

#include "qps/pql/query.h"
#include "qps/query_evaluator/context.h"
#include "table.h"

class Database {
 private:
  Context *ctx_;

  SynonymSet all_synonyms_;

  std::vector<Table *> unmerged_tables_;

  std::vector<Table *> merged_tables_;

 public:
  explicit Database(Context *ctx, SynonymSet all_synonyms) : ctx_(ctx), all_synonyms_(std::move(all_synonyms)) {
  };
  static std::vector<Table *> Empty(std::vector<QuerySynonym *> synonyms);
  void AddTable(Table *table);
  void MergeTables();
  bool IsEmpty() const;
  bool HasResults() const;
  Table *GetTable(const std::vector<QuerySynonym *> &synonyms);
};

