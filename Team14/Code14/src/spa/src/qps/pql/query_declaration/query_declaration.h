// Copyright 2022 CS3203 Team14. All rights reserved.

#pragma once

#include <utility>
#include <unordered_set>

#include "commons/entity.h"
#include "qps/pql/query_synonym/query_synonym.h"

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
  kString,
  kInteger,
};


class QueryDeclaration {
 private:
  DeclarationType type_;

 public:
  QueryDeclaration(DeclarationType type) : type_(std::move(type)) {};

  [[nodiscard]] DeclarationType * getType() const;
  bool operator==(const QueryDeclaration &other) const;
};

class StatementDeclaration : public QueryDeclaration {
 public:
  QuerySynonym query_synonym_;
  explicit StatementDeclaration(QuerySynonym query_synonym)
      : QueryDeclaration(DeclarationType::kStatement), query_synonym_(std::move(query_synonym)) {}
};

class ReadDeclaration : public QueryDeclaration {
 public:
  QuerySynonym query_synonym_;
  explicit ReadDeclaration(QuerySynonym query_synonym)
      : QueryDeclaration(DeclarationType::kRead), query_synonym_(std::move(query_synonym)) {}
};

class PrintDeclaration : public QueryDeclaration {
 public:
  QuerySynonym query_synonym_;
  explicit PrintDeclaration(QuerySynonym query_synonym)
      : QueryDeclaration(DeclarationType::kPrint), query_synonym_(std::move(query_synonym)) {}
};

class CallDeclaration : public QueryDeclaration {
 public:
  QuerySynonym query_synonym_;
  explicit CallDeclaration(QuerySynonym query_synonym)
      : QueryDeclaration(DeclarationType::kCall), query_synonym_(std::move(query_synonym)) {}
};

class WhileDeclaration : public QueryDeclaration {
 public:
  QuerySynonym query_synonym_;
  explicit WhileDeclaration(QuerySynonym query_synonym)
      : QueryDeclaration(DeclarationType::kWhile), query_synonym_(std::move(query_synonym)) {}
};
class IfDeclaration : public QueryDeclaration {
 public:
  QuerySynonym query_synonym_;
  explicit IfDeclaration(QuerySynonym query_synonym)
      : QueryDeclaration(DeclarationType::kIf), query_synonym_(std::move(query_synonym)) {}
};
class AssignDeclaration : public QueryDeclaration {
 public:
  QuerySynonym query_synonym_;
  explicit AssignDeclaration(QuerySynonym query_synonym)
      : QueryDeclaration(DeclarationType::kAssign), query_synonym_(std::move(query_synonym)) {}
};
class VariableDeclaration : public QueryDeclaration {
 public:
  QuerySynonym query_synonym_;
  explicit VariableDeclaration(QuerySynonym query_synonym)
      : QueryDeclaration(DeclarationType::kVariable), query_synonym_(std::move(query_synonym)) {}
};
class ConstantDeclaration : public QueryDeclaration {
 public:
  QuerySynonym query_synonym_;
  explicit ConstantDeclaration(QuerySynonym query_synonym)
      : QueryDeclaration(DeclarationType::kConstant), query_synonym_(std::move(query_synonym)) {}
};
class ProcedureDeclaration : public QueryDeclaration {
 public:
  QuerySynonym query_synonym_;
  explicit ProcedureDeclaration(QuerySynonym query_synonym)
      : QueryDeclaration(DeclarationType::kProcedure), query_synonym_(std::move(query_synonym)) {}
};
class WildCardDeclaration : public QueryDeclaration {
 public:
  explicit WildCardDeclaration()
  : QueryDeclaration(DeclarationType::kWildcard) {}
};
class StringDeclaration : public QueryDeclaration {
 public:
  std::string string_;
  explicit StringDeclaration(std::string string)
      : QueryDeclaration(DeclarationType::kString), string_(std::move(string)) {}
};
class IntegerDeclaration : public QueryDeclaration {
 public:
  int number_;
  explicit IntegerDeclaration(int number)
      : QueryDeclaration(DeclarationType::kInteger), number_(number) {}
};
