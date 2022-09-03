#pragma once

#include "../../qpl/query/query.h"
#include "../../qpl/query_declaration/query_declaration.h"
#include "../../qpl/command/command.h"
#include "../../qpl/query_relationship/query_relationship.h"
#include <vector>

class Query {
 public:
  explicit Query(const std::vector<QueryDeclaration>& declarationVector, const Command& command);
  std::vector<QueryDeclaration> declarationVector;
  Command command;
};
