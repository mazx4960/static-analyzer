// Copyright 2022 CS3203 Team14. All rights reserved.

#pragma once

#include <unordered_map>
#include "qps/pql/query_synonym.h"
#include "types.h"

using SynonymMap = std::unordered_map<QuerySynonym *, EntitySet>;

class Context {
 private:
  SynonymMap context_;

 public:
  Context() = default;
  EntitySet Get(QuerySynonym *synonym) const;
  void Set(QuerySynonym *synonym, EntitySet &entities);

  void LogStatistics();
};

