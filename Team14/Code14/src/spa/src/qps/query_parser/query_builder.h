// Copyright 2022 CS3203 Team14. All rights reserved.

#include <utility>
#include <vector>
#include <unordered_set>

#include "commons/lexer/token.h"
#include "commons/parser/parser_exceptions.h"
#include "qps/pql/query/query.h"

class QueryBuilder {
 private:
  std::vector<QueryDeclaration*> query_declarations_;
  std::vector<QueryCall*> query_calls_;

 public:
  explicit QueryBuilder();
  Query build();
  void withDeclarations(std::vector<QueryDeclaration*> query_declarations);
  void withQueryCalls(std::vector<QueryCall*> query_calls);
};

