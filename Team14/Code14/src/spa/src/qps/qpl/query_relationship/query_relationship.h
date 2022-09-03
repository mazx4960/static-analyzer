#pragma once

#include <utility>

#include "../../qpl/query/query.h"
#include "../../qpl/query_declaration/query_declaration.h"
#include "../../qpl/command/command.h"
#include "../../qpl/query_relationship/query_relationship.h"

enum class Clause {
  SUCH_THAT,
  PATTERN
};

class QueryRelationship {
 public:
  QueryRelationship(Clause clause, QueryDeclaration primaryDeclaration,
                    QueryDeclaration secondaryDeclaration);
  QueryDeclaration primaryDeclaration, secondaryDeclaration;
};

class SuchThatRelationship : public QueryRelationship {
 public:
  explicit SuchThatRelationship(QueryDeclaration primaryDeclaration,
  QueryDeclaration secondaryDeclaration)
      : QueryRelationship(Clause::SUCH_THAT, std::move(primaryDeclaration), std::move(secondaryDeclaration)) {}
};

class PatternRelationship : public QueryRelationship {
 public:
  explicit PatternRelationship(QueryDeclaration primaryDeclaration,
                               QueryDeclaration secondaryDeclaration)
      : QueryRelationship(Clause::PATTERN, std::move(primaryDeclaration), std::move(secondaryDeclaration)) {
  }
};
