#pragma once

#include "../../qpl/query/query.h"
#include "../../qpl/query_declaration/query_declaration.h"
#include "../../qpl/command/command.h"
#include "../../qpl/query_relationship/query_relationship.h"
#include <vector>

enum class CommandType { SELECT };


class Command {
 protected:
  Command(CommandType type, QueryDeclaration declaration, std::vector<QueryRelationship> relationshipsVector);

 public:
  Command();
  QueryDeclaration declaration;
  std::vector<QueryRelationship> relationshipsVector;
};

class SelectCommand : public Command {
 public:
  explicit SelectCommand(QueryDeclaration declaration,
                         std::vector<QueryRelationship> abstractionVector)
      : Command(CommandType::SELECT, declaration, abstractionVector) {
  }
};
