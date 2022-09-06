// Copyright 2022 CS3203 Team14. All rights reserved.

#pragma once

#include "qps/pql/query/query.h"

class QueryParser {
 public:
  static Query parse(std::ifstream *query_stream);
};

