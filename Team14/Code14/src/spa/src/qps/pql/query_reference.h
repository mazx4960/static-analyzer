// Copyright 2022 CS3203 Team14. All rights reserved.

#pragma once

#include <unordered_set>
#include <utility>
#include "types.h"
#include "query_synonym.h"
#include "qps/pql/pql_interfaces.h"

class QueryReference {
 private:
  ReferenceType reference_type_;
 protected:
  explicit QueryReference(ReferenceType reference_type) : reference_type_(reference_type) {
  };

 public:
  [[nodiscard]] virtual int getUses() const = 0;
  [[nodiscard]] ReferenceType getRefType() const;
  [[nodiscard]] virtual std::string toString() const = 0;
  virtual bool operator==(const QueryReference &other) const;
  virtual bool operator==(const QueryReference *other) const;
};

class WildcardReference : public QueryReference {
 private:
  EntityType entity_type_;

 public:
  explicit WildcardReference(EntityType wildcard_type = EntityType::kUnknown)
      : QueryReference(ReferenceType::kWildcard), entity_type_(wildcard_type) {
  };
  [[nodiscard]] inline int getUses() const override {
    return 0;
  }
  [[nodiscard]] EntityType getEntityType() const;
  [[nodiscard]] std::string toString() const override;
  bool operator==(const QueryReference &other) const override;
  bool operator==(const QueryReference *other) const override;
};

class IdentReference : public QueryReference {
 private:
  std::string value_;

 public:
  explicit IdentReference(std::string value) : QueryReference(ReferenceType::kIdent), value_(std::move(value)) {
  }
  [[nodiscard]] inline int getUses() const override {
    return 0;
  }
  [[nodiscard]] std::string getValue() const;
  [[nodiscard]] std::string toString() const override;
  bool operator==(const QueryReference &other) const override;
  bool operator==(const QueryReference *other) const override;
};

class IntegerReference : public QueryReference {
 private:
  std::string value_;

 public:
  explicit IntegerReference(std::string value) : QueryReference(ReferenceType::kInteger), value_(std::move(value)) {
  }
  inline int getUses() const override {
    return 0;
  }

  [[nodiscard]] std::string getValue() const;
  [[nodiscard]] std::string toString() const override;
  bool operator==(const QueryReference &other) const override;
  bool operator==(const QueryReference *other) const override;
};

class ElemReference : public QueryReference {
 public:
  explicit ElemReference(ReferenceType reference_type) : QueryReference(reference_type) {
  }
  [[nodiscard]] virtual QuerySynonym *getSynonym() const = 0;
};

class SynonymReference : public ElemReference {
 private:
  QuerySynonym *query_synonym_;

  bool is_boolean_ref_;
  int uses_ = 0;

 public:
  explicit SynonymReference(QuerySynonym *query_synonym)
      : ElemReference(ReferenceType::kSynonym), query_synonym_(query_synonym) {
  };
  void incrementUses(int uses = 1);
  [[nodiscard]] int getUses() const override;
  [[nodiscard]] QuerySynonym *getSynonym() const override;
  [[nodiscard]] std::string toString() const override;
  bool operator==(const QueryReference &other) const override;
  bool operator==(const QueryReference *other) const override;
};

class AttrReference : public ElemReference, public ICheckSemantics {
 private:
  QuerySynonym *synonym_;
  AttributeType attribute_type_;

 public:

  explicit AttrReference(QuerySynonym *synonym, AttributeType attribute_type)
      : ElemReference(ReferenceType::kAttr), synonym_(synonym), attribute_type_(attribute_type) {
  };
  [[nodiscard]] inline int getUses() const override {
    return 0;
  }
  [[nodiscard]] QuerySynonym *getSynonym() const override;
  [[nodiscard]] AttributeType getAttributeType() const;
  std::string getAttribute(Entity *entity) const;
  [[nodiscard]] bool isSemanticallyCorrect() const override;
  [[nodiscard]] std::string toString() const override;
  bool operator==(const QueryReference &other) const override;
  bool operator==(const QueryReference *other) const override;
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
