// Copyright 2022 CS3203 Team14. All rights reserved.

#pragma once

#include <unordered_set>
#include <utility>

#include "commons/entity.h"
#include "qps/exceptions.h"
#include "query_synonym.h"
#include "qps/pql/interface/check_syntax.h"

using EntityPointerUnorderedSet = std::unordered_set<Entity *, EntityHashFunction, EntityPointerEquality>;

enum class ReferenceType {
  kSynonym,
  kWildcard,
  kInteger,
  kIdent
};

class QueryReference : public ICheckSyntax{
 private:
  ReferenceType reference_type_;
  EntityType entity_type_;
  EntityPointerUnorderedSet context_;

 protected:
  explicit QueryReference(ReferenceType reference_type, EntityType entity_type)
      : reference_type_(reference_type), entity_type_(entity_type) {
  };

 public:
  ReferenceType getRefType() const;
  EntityType getEntityType() const;
  EntityPointerUnorderedSet getContext() const;
  void setContext(EntityPointerUnorderedSet);

  virtual bool operator==(const QueryReference &other) const = 0;
  virtual bool operator==(const QueryReference *other) const = 0;
  virtual std::string getReferenceValue() const = 0;
  virtual bool isStmtRef() const = 0;
  virtual bool isEntRef() const = 0;
  virtual std::string toString() const = 0;
  bool isSyntacticallyCorrect() const override = 0;
  void setEntityType(EntityType entity_type);
};

class WildcardReference : public QueryReference {
 public:
  explicit WildcardReference(EntityType wildcard_type = EntityType::kUnknown)
      : QueryReference(ReferenceType::kWildcard, wildcard_type) {
  };
  bool operator==(const QueryReference &other) const override;
  bool operator==(const QueryReference *other) const override;
  std::string getReferenceValue() const override;
  bool isStmtRef() const override;
  bool isEntRef() const override;
  bool isSyntacticallyCorrect() const override;
  std::string toString() const override;
};

class IdentReference : public QueryReference {
 private:
  std::string value_;
 public:
  explicit IdentReference(std::string value, EntityType entity_type = EntityType::kUnknown)
      : QueryReference(ReferenceType::kIdent, entity_type), value_(std::move(value)) {
  }
  bool operator==(const QueryReference &other) const override;
  bool operator==(const QueryReference *other) const override;
  std::string getReferenceValue() const override;
  bool isStmtRef() const override;
  bool isEntRef() const override;
  bool isSyntacticallyCorrect() const override;
  std::string toString() const override;
};

class IntegerReference : public QueryReference {
 private:
  std::string value_;
 public:
  explicit IntegerReference(std::string value, EntityType entity_type = EntityType::kStatement)
      : QueryReference(ReferenceType::kInteger, entity_type), value_(std::move(value)) {
  }
  bool operator==(const QueryReference &other) const override;
  bool operator==(const QueryReference *other) const override;
  std::string getReferenceValue() const override;
  bool isStmtRef() const override;
  bool isEntRef() const override;
  bool isSyntacticallyCorrect() const override;
  std::string toString() const override;
};

class SynonymReference : public QueryReference {
 private:
  QuerySynonym *query_synonym_;

 public:
  explicit SynonymReference(QuerySynonym *query_synonym, EntityType entity_type = EntityType::kUnknown)
      : QueryReference(ReferenceType::kSynonym, entity_type), query_synonym_(query_synonym) {
  };
  bool operator==(const QueryReference &other) const override;
  bool operator==(const QueryReference *other) const override;
  QuerySynonym *getSynonym() const;
  std::string getReferenceValue() const override;
  bool isStmtRef() const override;
  bool isEntRef() const override;
  bool isSyntacticallyCorrect() const override;
  std::string toString() const override;
};

class StatementDeclaration : public SynonymReference {
 public:
  explicit StatementDeclaration(QuerySynonym *query_synonym)
      : SynonymReference(query_synonym, EntityType::kStatement) {
  };
};

class IfDeclaration : public SynonymReference {
 public:
  explicit IfDeclaration(QuerySynonym *query_synonym)
      : SynonymReference(query_synonym, EntityType::kIfStmt) {
  };
};

class ReadDeclaration : public SynonymReference {
 public:
  explicit ReadDeclaration(QuerySynonym *query_synonym)
      : SynonymReference(query_synonym, EntityType::kReadStmt) {
  };
};

class CallDeclaration : public SynonymReference {
 public:
  explicit CallDeclaration(QuerySynonym *query_synonym)
      : SynonymReference(query_synonym, EntityType::kCallStmt) {
  };
};

class PrintDeclaration : public SynonymReference {
 public:
  explicit PrintDeclaration(QuerySynonym *query_synonym)
      : SynonymReference(query_synonym, EntityType::kPrintStmt) {
  };
};

class WhileDeclaration : public SynonymReference {
 public:
  explicit WhileDeclaration(QuerySynonym *query_synonym)
      : SynonymReference(query_synonym, EntityType::kWhileStmt) {
  };
};

class AssignDeclaration : public SynonymReference {
 public:
  explicit AssignDeclaration(QuerySynonym *query_synonym)
      : SynonymReference(query_synonym, EntityType::kAssignStmt) {
  };
};

class VariableDeclaration : public SynonymReference {
 public:
  explicit VariableDeclaration(QuerySynonym *query_synonym)
      : SynonymReference(query_synonym, EntityType::kVariable) {
  };
};

class ConstantDeclaration : public SynonymReference {
 public:
  explicit ConstantDeclaration(QuerySynonym *query_synonym)
      : SynonymReference(query_synonym, EntityType::kConstant) {
  };
};

class ProcedureDeclaration : public SynonymReference {
 public:
  explicit ProcedureDeclaration(QuerySynonym *query_synonym)
      : SynonymReference(query_synonym, EntityType::kProcedure) {
  };
};

struct SynonymReferenceHashFunction {
  size_t operator()(const SynonymReference &declaration) const {
    return QuerySynonymHashFunction().operator()(declaration.getSynonym());
  }
  size_t operator()(const SynonymReference *declaration) const {
    return QuerySynonymHashFunction().operator()(declaration->getSynonym());
  }
};

struct SynonymReferencePointerEquality {
  bool operator()(const SynonymReference *lhs, const SynonymReference *rhs) const {
    return (*lhs) == (*rhs);
  }
};
