// Copyright 2022 CS3203 Team14. All rights reserved.

#pragma once

#include <unordered_set>
#include <utility>

#include "commons/entity.h"
#include "qps/exceptions.h"
#include "query_synonym.h"
#include "qps/pql/pql_interfaces.h"
#include "types.h"

class QueryReference : public ICheckSyntax, public IStmtRef, public IEntRef, public IAttrCompareRef {
 private:
  ReferenceType reference_type_;

  EntityType entity_type_;

  EntityPointerUnorderedSet context_;

 protected:
  explicit QueryReference(ReferenceType reference_type, EntityType entity_type)
      : reference_type_(reference_type), entity_type_(entity_type) {
  }
  explicit QueryReference(ReferenceType reference_type) : reference_type_(reference_type) {
  };

 public:

  virtual int getUses() const = 0;
  virtual void setContext(EntityPointerUnorderedSet);
  virtual void setEntityType(EntityType entity_type);

  [[nodiscard]] ReferenceType getRefType() const;
  [[nodiscard]] virtual EntityType getEntityType() const;
  [[nodiscard]] virtual EntityPointerUnorderedSet getContext() const;
  [[nodiscard]] virtual std::string getReferenceValue() const = 0;
  [[nodiscard]] virtual std::string toString() const = 0;
  [[nodiscard]] bool isSyntacticallyCorrect() const override = 0;
  [[nodiscard]] bool isStmtRef() const override;
  [[nodiscard]] bool isEntRef() const override;
  [[nodiscard]] bool isAttrCompareRef() const override;

  virtual bool operator==(const QueryReference &other) const = 0;
};

class WildcardReference : public QueryReference {
 public:
  explicit WildcardReference(EntityType wildcard_type = EntityType::kUnknown)
      : QueryReference(ReferenceType::kWildcard, wildcard_type) {
  };

  inline int getUses() const override {
    return 0;
  }

  [[nodiscard]] std::string getReferenceValue() const override;
  [[nodiscard]] bool isStmtRef() const override;
  [[nodiscard]] bool isEntRef() const override;
  [[nodiscard]] bool isSyntacticallyCorrect() const override;
  [[nodiscard]] std::string toString() const override;

  bool operator==(const QueryReference &other) const override;
};

class IdentReference : public QueryReference {
 private:
  std::string value_;

 public:
  explicit IdentReference(std::string value, EntityType entity_type = EntityType::kUnknown) : QueryReference(
      ReferenceType::kIdent,
      entity_type), value_(std::move(value)) {
  }

  inline int getUses() const override {
    return 0;
  }

  [[nodiscard]]  std::string getReferenceValue() const override;
  [[nodiscard]] bool isEntRef() const override;
  [[nodiscard]] bool isAttrCompareRef() const override;
  [[nodiscard]] bool isSyntacticallyCorrect() const override;
  [[nodiscard]] std::string toString() const override;

  bool operator==(const QueryReference &other) const override;
};

class IntegerReference : public QueryReference {
 private:
  std::string value_;

 public:
  explicit IntegerReference(std::string value, EntityType entity_type = EntityType::kStatement) : QueryReference(
      ReferenceType::kInteger,
      entity_type), value_(std::move(value)) {
  }

  inline int getUses() const override {
    return 0;
  }

  [[nodiscard]] std::string getReferenceValue() const override;
  [[nodiscard]] bool isStmtRef() const override;
  [[nodiscard]] bool isAttrCompareRef() const override;
  [[nodiscard]] bool isSyntacticallyCorrect() const override;
  [[nodiscard]] std::string toString() const override;

  bool operator==(const QueryReference &other) const override;
};

class ElemReference {
 public:
  [[nodiscard]] virtual ReferenceType getRefType() const = 0;
  [[nodiscard]] virtual std::string toString() const = 0;
  [[nodiscard]] virtual QuerySynonym *getSynonym() const = 0;
  [[nodiscard]] virtual EntityPointerUnorderedSet getContext() const = 0;
};

class SynonymReference : public QueryReference, public IBooleanRef, public ElemReference {
 private:
  QuerySynonym *query_synonym_;

  bool is_boolean_ref_;
  int uses_ = 0;

 public:
  explicit SynonymReference(QuerySynonym *query_synonym, EntityType entity_type = EntityType::kUnknown)
      : QueryReference(ReferenceType::kSynonym, entity_type), query_synonym_(query_synonym) {
  };

  void incrementUses(int uses = 1);
  int getUses() const override;
  void setBooleanRef(bool is_boolean_Ref) override;

  [[nodiscard]] ReferenceType getRefType() const override;
  [[nodiscard]] QuerySynonym *getSynonym() const override;
  [[nodiscard]] std::string getReferenceValue() const override;
  [[nodiscard]] EntityPointerUnorderedSet getContext() const override;
  [[nodiscard]] bool isStmtRef() const override;
  [[nodiscard]] bool isEntRef() const override;
  [[nodiscard]] bool isSyntacticallyCorrect() const override;
  [[nodiscard]] bool isBooleanRef() const override;
  [[nodiscard]] std::string toString() const override;

  bool operator==(const QueryReference &other) const override;
};

class AttrReference : public QueryReference, public ElemReference, public ICheckSemantics {
 private:
  SynonymReference *synonym_reference_;

  AttributeType attribute_type_;

 public:
  explicit AttrReference(SynonymReference *synonym_reference, AttributeType attribute_type) : QueryReference(
      ReferenceType::kAttr,
      EntityType::kUnknown),
                                                                                              synonym_reference_(
                                                                                                  synonym_reference),
                                                                                              attribute_type_(
                                                                                                  attribute_type) {
  };

  inline int getUses() const override {
    return 0;
  }
  void setContext(EntityPointerUnorderedSet context) override;
  void setEntityType(EntityType entity_type) override;
  void setSynonymReference(SynonymReference *synonym_reference);

  [[nodiscard]] SynonymReference *getSynonymReference() const;
  [[nodiscard]] QuerySynonym *getSynonym() const override;
  [[nodiscard]] ReferenceType getRefType() const override;
  [[nodiscard]] AttributeType getAttributeType() const;
  [[nodiscard]] std::string getAttribute(Entity *entity) const;
  [[nodiscard]] EntityType getEntityType() const override;
  [[nodiscard]] EntityPointerUnorderedSet getContext() const override;
  [[nodiscard]] std::string getReferenceValue() const override;
  [[nodiscard]] bool isStmtRef() const override;
  [[nodiscard]] bool isEntRef() const override;
  [[nodiscard]] bool isAttrCompareRef() const override;
  [[nodiscard]] bool isSyntacticallyCorrect() const override;
  [[nodiscard]] bool isSemanticallyCorrect() const override;
  [[nodiscard]] std::string toString() const override;

  bool operator==(const QueryReference &other) const override;
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
