// Copyright 2022 CS3203 Team14. All rights reserved.

#pragma once

#include <unordered_set>
#include <utility>

#include "commons/entity.h"
#include "qps/exceptions.h"
#include "query_synonym.h"

using EntityPointerUnorderedSet = std::unordered_set<Entity *, EntityHashFunction, EntityPointerEquality>;

class QueryDeclaration {
 private:
  EntityType type_;

  QuerySynonym *query_synonym_ = QuerySynonym::empty();

  std::string string_;

 protected:
  EntityPointerUnorderedSet context_;

 public:
  explicit QueryDeclaration(EntityType type) : type_(std::move(type)) {
  };
  QueryDeclaration(EntityType type, QuerySynonym *query_synonym)
      : type_(std::move(type)),
        query_synonym_(std::move(query_synonym)),
        string_(query_synonym->toString()) {
  };
  QueryDeclaration(EntityType type, std::string string)
      : type_(std::move(type)),
        string_(std::move(string)),
        query_synonym_(QuerySynonym::empty()) {
  };

  [[nodiscard]] EntityType getType() const;
  [[nodiscard]] QuerySynonym *getSynonym() const;
  [[nodiscard]] std::string toString() const;
  [[nodiscard]] EntityPointerUnorderedSet getContext() const;
  void removeEntityFromContext(Entity *entity);
  void intersectContext(const EntityPointerUnorderedSet &other_context);
  void setContext(EntityPointerUnorderedSet);
  bool operator==(const QueryDeclaration &) const;
  bool operator==(const QueryDeclaration *) const;
};

// Pre-call declaration of "stmt x;" where x is the synonym
class StatementDeclaration : public QueryDeclaration {
 public:
  explicit StatementDeclaration(QuerySynonym *query_synonym)
      : QueryDeclaration(EntityType::kStatement, std::move(query_synonym)) {
  }
};

// Pre-call declaration of "read x;" where x is the synonym
class ReadDeclaration : public QueryDeclaration {
 public:
  explicit ReadDeclaration(QuerySynonym *query_synonym)
      : QueryDeclaration(EntityType::kReadStmt, std::move(query_synonym)) {
  }
};

// Pre-call declaration of "print x;" where x is the synonym
class PrintDeclaration : public QueryDeclaration {
 public:
  explicit PrintDeclaration(QuerySynonym *query_synonym)
      : QueryDeclaration(EntityType::kPrintStmt, std::move(query_synonym)) {
  }
};

// Pre-call declaration of "call x;" where x is the synonym
class CallDeclaration : public QueryDeclaration {
 public:
  explicit CallDeclaration(QuerySynonym *query_synonym)
      : QueryDeclaration(EntityType::kCallStmt, std::move(query_synonym)) {
  }
};

// Pre-call declaration of "if x;" where x is the synonym
class WhileDeclaration : public QueryDeclaration {
 public:
  explicit WhileDeclaration(QuerySynonym *query_synonym)
      : QueryDeclaration(EntityType::kWhileStmt, std::move(query_synonym)) {
  }
};

// Pre-call declaration of "if x;" where x is the synonym
class IfDeclaration : public QueryDeclaration {
 public:
  explicit IfDeclaration(QuerySynonym *query_synonym)
      : QueryDeclaration(EntityType::kIfStmt, std::move(query_synonym)) {
  }
};

// Pre-call declaration of "assign x;" where x is the synonym
class AssignDeclaration : public QueryDeclaration {
 public:
  explicit AssignDeclaration(QuerySynonym *query_synonym)
      : QueryDeclaration(EntityType::kAssignStmt, std::move(query_synonym)) {
  }
};

// Pre-call declaration of "variable x;" where x is the synonym
class VariableDeclaration : public QueryDeclaration {
 public:
  explicit VariableDeclaration(QuerySynonym *query_synonym)
      : QueryDeclaration(EntityType::kVariable, std::move(query_synonym)) {
  }
};

// Pre-call declaration of "constant x;" where x is the synonym
class ConstantDeclaration : public QueryDeclaration {
 public:
  explicit ConstantDeclaration(QuerySynonym *query_synonym)
      : QueryDeclaration(EntityType::kConstant, std::move(query_synonym)) {
  }
};

// Pre-call declaration of "procedure x;" where x is the synonym
class ProcedureDeclaration : public QueryDeclaration {
 public:
  explicit ProcedureDeclaration(QuerySynonym *query_synonym)
      : QueryDeclaration(EntityType::kProcedure, std::move(query_synonym)) {
  }
};

// Inline declaration of  Wildcard "_"
class WildCardEntDeclaration : public QueryDeclaration {
 public:
  explicit WildCardEntDeclaration() : QueryDeclaration(EntityType::kWildcardEnt) {
  }
};

// Inline declaration of  Wildcard "_"
class WildCardStmtDeclaration : public QueryDeclaration {
 public:
  explicit WildCardStmtDeclaration() : QueryDeclaration(EntityType::kWildcardStmt) {
  }
};

// Inline declaration of Expression "(x + (y * z))"
class ExpressionDeclaration : public QueryDeclaration {
 public:
  explicit ExpressionDeclaration(std::string string) : QueryDeclaration(EntityType::kExpression, std::move(string)) {
  }
};

// Inline declaration of Expression _"(x + (y * z))"_
class WildCardExpressionDeclaration : public QueryDeclaration {
 public:
  explicit WildCardExpressionDeclaration(std::string string)
      : QueryDeclaration(EntityType::kWildcardExpression, std::move(string)) {
  }
};

// Inline declaration of string eg. Modifies("x", v)
class IdentDeclaration : public QueryDeclaration {
 public:
  explicit IdentDeclaration(std::string string) : QueryDeclaration(EntityType::kString, std::move(string)) {
  }
};

// Inline declaration of integer eg. Modifies(3, v)
class IntegerDeclaration : public QueryDeclaration {
 public:
  explicit IntegerDeclaration(std::string number) : QueryDeclaration(EntityType::kInteger, std::move(number)) {
  }
};

struct QueryDeclarationHashFunction {
  size_t operator()(const QueryDeclaration &declaration) const {
    return QuerySynonymHashFunction().operator()(declaration.getSynonym());
  }
  size_t operator()(const QueryDeclaration *declaration) const {
    return QuerySynonymHashFunction().operator()(declaration->getSynonym());
  }
};

struct QueryDeclarationPointerEquality {
  bool operator()(const QueryDeclaration *lhs, const QueryDeclaration *rhs) const {
    if (lhs->getType() == EntityType::kWildcardEnt || rhs->getType() == EntityType::kWildcardEnt ||
        lhs->getType() == EntityType::kWildcardStmt || rhs->getType() == EntityType::kWildcardStmt) { return true; }
    return (*lhs) == (*rhs);
  }
};
