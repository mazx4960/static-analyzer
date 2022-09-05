// Copyright 2022 CS3203 Team14. All rights reserved.

#include <vector>
#include "commons/lexer/token.h"
#include "qps/pql/query/query.h"

class QueryBuilder {
 private:
  std::vector<Token*> tokens_;
 public:
  explicit QueryBuilder(std::vector<Token*> tokens);
  static Query build();

};

