// Copyright 2022 CS3203 Team14. All rights reserved.

#pragma once

#include <unordered_set>
#include <utility>

#include "commons/entity.h"
#include "query_synonym.h"

enum class DeclarationType {
  kStatement,
  kRead,
  kPrint,
  kCall,
  kWhile,
  kIf,
  kAssign,
  kVariable,
  kConstant,
  kProcedure,
  kWildcard,
  kExpression,
  kString,
  kInteger,
};

class QueryDeclaration {
 private:
  DeclarationType type_;

  QuerySynonym *query_synonym_;

  std::string string_;

  int number_;

 protected:
  std::unordered_set<Entity *> context_;

 public:
  explicit QueryDeclaration(DeclarationType type) : type_(std::move(type)) {};
  QueryDeclaration(DeclarationType type, QuerySynonym *query_synonym)
      : type_(std::move(type)),
        query_synonym_(std::move(query_synonym)) {};
  QueryDeclaration(DeclarationType type, std::string string) : type_(std::move(type)), string_(std::move(string)) {};
  QueryDeclaration(DeclarationType type, int number) : type_(std::move(type)), number_(std::move(number)) {};

  [[nodiscard]] DeclarationType getType() const;
  [[nodiscard]] QuerySynonym *getSynonym() const;
  [[nodiscard]] std::string getString() const;
  [[nodiscard]] int getNumber() const;
  [[nodiscard]] std::unordered_set<Entity *> getContext() const;
  void setContext(std::unordered_set<Entity *>);
  bool operator==(const QueryDeclaration &other) const;
};

// Pre-call declaration of "stmt x;" where x is the synonym
class StatementDeclaration : public QueryDeclaration {
 public:
  explicit StatementDeclaration(QuerySynonym *query_synonym)
      : QueryDeclaration(DeclarationType::kStatement, std::move(query_synonym)) {}
};

// Pre-call declaration of "read x;" where x is the synonym
class ReadDeclaration : public QueryDeclaration {
 public:
  explicit ReadDeclaration(QuerySynonym *query_synonym)
      : QueryDeclaration(DeclarationType::kRead, std::move(query_synonym)) {}
};

// Pre-call declaration of "print x;" where x is the synonym
class PrintDeclaration : public QueryDeclaration {
 public:
  explicit PrintDeclaration(QuerySynonym *query_synonym)
      : QueryDeclaration(DeclarationType::kPrint, std::move(query_synonym)) {}
};

// Pre-call declaration of "call x;" where x is the synonym
class CallDeclaration : public QueryDeclaration {
 public:
  explicit CallDeclaration(QuerySynonym *query_synonym)
      : QueryDeclaration(DeclarationType::kCall, std::move(query_synonym)) {}
};

// Pre-call declaration of "if x;" where x is the synonym
class WhileDeclaration : public QueryDeclaration {
 public:
  explicit WhileDeclaration(QuerySynonym *query_synonym)
      : QueryDeclaration(DeclarationType::kWhile, std::move(query_synonym)) {}
};

// Pre-call declaration of "if x;" where x is the synonym
class IfDeclaration : public QueryDeclaration {
 public:
  explicit IfDeclaration(QuerySynonym *query_synonym)
      : QueryDeclaration(DeclarationType::kIf, std::move(query_synonym)) {}
};

// Pre-call declaration of "assign x;" where x is the synonym
class AssignDeclaration : public QueryDeclaration {
 public:
  explicit AssignDeclaration(QuerySynonym *query_synonym)
      : QueryDeclaration(DeclarationType::kAssign, std::move(query_synonym)) {}
};

// Pre-call declaration of "variable x;" where x is the synonym
class VariableDeclaration : public QueryDeclaration {
 public:
  explicit VariableDeclaration(QuerySynonym *query_synonym)
      : QueryDeclaration(DeclarationType::kVariable, std::move(query_synonym)) {}
};

// Pre-call declaration of "constant x;" where x is the synonym
class ConstantDeclaration : public QueryDeclaration {
 public:
  explicit ConstantDeclaration(QuerySynonym *query_synonym)
      : QueryDeclaration(DeclarationType::kConstant, std::move(query_synonym)) {}
};

// Pre-call declaration of "procedure x;" where x is the synonym
class ProcedureDeclaration : public QueryDeclaration {
 public:
  explicit ProcedureDeclaration(QuerySynonym *query_synonym)
      : QueryDeclaration(DeclarationType::kProcedure, std::move(query_synonym)) {}
};

// Inline declaration of  Wildcard "_"
class WildCardDeclaration : public QueryDeclaration {
 public:
  explicit WildCardDeclaration() : QueryDeclaration(DeclarationType::kWildcard) {}
};

// Inline declaration of Expression "(x + (y * z))"
class ExpressionDeclaration : public QueryDeclaration {
 public:
  explicit ExpressionDeclaration(std::string string)
      : QueryDeclaration(DeclarationType::kExpression, std::move(string)) {}
};

// Inline declaration of string eg. Modifies("x", v)
class StringDeclaration : public QueryDeclaration {
 public:
  explicit StringDeclaration(std::string string) : QueryDeclaration(DeclarationType::kString, std::move(string)) {}
};

// Inline declaration of integer eg. Modifies(3, v)
class IntegerDeclaration : public QueryDeclaration {
 public:
  explicit IntegerDeclaration(int number) : QueryDeclaration(DeclarationType::kInteger, number) {}
};

class DeclarationTypeAdaptor {
 public:
  static bool canConvertToEntityType(DeclarationType);
  static bool canConvertToStatementType(DeclarationType);

  static EntityType toEntityType(DeclarationType);
  static EntityType toStatementType(DeclarationType);

  static DeclarationType toDeclarationType(EntityType);
};

class QueryDeclarationHashFunction {
 public:
  size_t operator()(const QueryDeclaration &declaration) const;
};