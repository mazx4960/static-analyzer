// Copyright 2022 CS3203 Team14. All rights reserved.

#pragma once

#include <unordered_set>
#include <utility>

#include "commons/entity.h"
#include "qps/exceptions.h"
#include "query_synonym.h"
#include "qps/pql/interface/check_syntax.h"
#include "qps/pql/interface/query_elem.h"
#include "qps/pql/interface/stmt_ref.h"
#include "qps/pql/interface/ent_ref.h"
#include "qps/pql/interface/boolean_ref.h"
#include "types.h"

class QueryReference : public ICheckSyntax, public IStmtRef, public IEntRef {
 private:
  ReferenceType reference_type_;
  EntityType entity_type_;
  EntityPointerUnorderedSet context_;
 protected:
  explicit QueryReference(ReferenceType reference_type, EntityType entity_type)
      : reference_type_(reference_type), entity_type_(entity_type) {
  };
  explicit QueryReference(ReferenceType reference_type) : reference_type_(reference_type) {
  };

 public:
  ReferenceType getRefType() const;

  virtual EntityType getEntityType() const;
  virtual EntityPointerUnorderedSet getContext() const;
  virtual void setContext(EntityPointerUnorderedSet);
  virtual void setEntityType(EntityType entity_type);
  virtual bool operator==(const QueryReference &other) const = 0;
  virtual bool operator==(const QueryReference *other) const = 0;
  virtual std::string getReferenceValue() const = 0;
  virtual std::string toString() const = 0;

  bool isSyntacticallyCorrect() const override = 0;
  bool isStmtRef() const override;
  bool isEntRef() const override;
};

class WildcardReference : public QueryReference {
 public:
  explicit WildcardReference(EntityType wildcard_type = EntityType::kUnknown) : QueryReference(ReferenceType::kWildcard,
                                                                                               wildcard_type) {
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
  explicit IdentReference(std::string value, EntityType entity_type = EntityType::kUnknown) : QueryReference(
      ReferenceType::kIdent,
      entity_type), value_(std::move(value)) {
  }
  bool operator==(const QueryReference &other) const override;
  bool operator==(const QueryReference *other) const override;
  std::string getReferenceValue() const override;
  bool isEntRef() const override;
  bool isSyntacticallyCorrect() const override;
  std::string toString() const override;
};

class IntegerReference : public QueryReference {
 private:
  std::string value_;
 public:
  explicit IntegerReference(std::string value, EntityType entity_type = EntityType::kStatement) : QueryReference(
      ReferenceType::kInteger,
      entity_type), value_(std::move(value)) {
  }
  bool operator==(const QueryReference &other) const override;
  bool operator==(const QueryReference *other) const override;
  std::string getReferenceValue() const override;
  bool isStmtRef() const override;
  bool isSyntacticallyCorrect() const override;
  std::string toString() const override;
};

class SynonymReference : public QueryReference, public IBooleanRef {
 private:
  QuerySynonym *query_synonym_;
  bool is_boolean_ref_;

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
  bool isBooleanRef() const override;
  void setBooleanRef(bool is_boolean_Ref) override;
  std::string toString() const override;
};

class ElemReference : public QueryReference {
 private:
  SynonymReference *synonym_reference_;
  AttributeType attribute_type_;

 public:
  explicit ElemReference(SynonymReference *synonym_reference, AttributeType attribute_type) : QueryReference(
      ReferenceType::kElem,
      EntityType::kUnknown), synonym_reference_(synonym_reference), attribute_type_(attribute_type) {
  };
  bool operator==(const QueryReference &other) const override;
  bool operator==(const QueryReference *other) const override;
  SynonymReference *getSynonymReference() const;
  AttributeType getAttribute() const;
  EntityType getEntityType() const override;
  EntityPointerUnorderedSet getContext() const override;
  void setContext(EntityPointerUnorderedSet context) override;
  void setEntityType(EntityType entity_type) override;
  void setSynonymReference(SynonymReference *synonym_reference);
  std::string getReferenceValue() const override;
  bool isStmtRef() const override;
  bool isEntRef() const override;
  bool isSyntacticallyCorrect() const override;
  std::string toString() const override;
};

class StatementDeclaration : public SynonymReference {
 public:
  explicit StatementDeclaration(QuerySynonym *query_synonym) : SynonymReference(query_synonym, EntityType::kStatement) {
  };
};

class IfDeclaration : public SynonymReference {
 public:
  explicit IfDeclaration(QuerySynonym *query_synonym) : SynonymReference(query_synonym, EntityType::kIfStmt) {
  };
};

class ReadDeclaration : public SynonymReference {
 public:
  explicit ReadDeclaration(QuerySynonym *query_synonym) : SynonymReference(query_synonym, EntityType::kReadStmt) {
  };
};

class CallDeclaration : public SynonymReference {
 public:
  explicit CallDeclaration(QuerySynonym *query_synonym) : SynonymReference(query_synonym, EntityType::kCallStmt) {
  };
};

class PrintDeclaration : public SynonymReference {
 public:
  explicit PrintDeclaration(QuerySynonym *query_synonym) : SynonymReference(query_synonym, EntityType::kPrintStmt) {
  };
};

class WhileDeclaration : public SynonymReference {
 public:
  explicit WhileDeclaration(QuerySynonym *query_synonym) : SynonymReference(query_synonym, EntityType::kWhileStmt) {
  };
};

class AssignDeclaration : public SynonymReference {
 public:
  explicit AssignDeclaration(QuerySynonym *query_synonym) : SynonymReference(query_synonym, EntityType::kAssignStmt) {
  };
};

class VariableDeclaration : public SynonymReference {
 public:
  explicit VariableDeclaration(QuerySynonym *query_synonym) : SynonymReference(query_synonym, EntityType::kVariable) {
  };
};

class ConstantDeclaration : public SynonymReference {
 public:
  explicit ConstantDeclaration(QuerySynonym *query_synonym) : SynonymReference(query_synonym, EntityType::kConstant) {
  };
};

class ProcedureDeclaration : public SynonymReference {
 public:
  explicit ProcedureDeclaration(QuerySynonym *query_synonym) : SynonymReference(query_synonym, EntityType::kProcedure) {
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
