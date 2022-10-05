// Copyright 2022 CS3203 Team14. All rights reserved.

#pragma once

#include <unordered_set>
#include <utility>

#include "commons/entity.h"
#include "qps/exceptions.h"
#include "query_synonym.h"

using EntityPointerUnorderedSet = std::unordered_set<Entity *, EntityHashFunction, EntityPointerEquality>;

enum class DeclarationType {
  kSynonym,
  kStatic
};

class QueryDeclaration {
 private:
  DeclarationType declaration_type_;
  EntityType entity_type_;

 protected:
  EntityPointerUnorderedSet context_;
  explicit QueryDeclaration(DeclarationType declaration_type, EntityType entity_type)
      : declaration_type_(declaration_type), entity_type_(entity_type) {
  };

 public:
  DeclarationType getDeclarationType() const;
  EntityType getEntityType() const;
  EntityPointerUnorderedSet getContext() const;
  void setContext(EntityPointerUnorderedSet);

  virtual bool operator==(const QueryDeclaration &other) const;
  virtual bool operator==(const QueryDeclaration *other) const;
  virtual std::string toString() const;
};

// Synonym declaration of "stmt x;" where x is the synonym
class SynonymDeclaration : public QueryDeclaration {
 private:
  QuerySynonym *query_synonym_;
 public:
  explicit SynonymDeclaration(QuerySynonym *query_synonym, EntityType entity_type = EntityType::kWildcard)
      : QueryDeclaration(DeclarationType::kSynonym, entity_type), query_synonym_(query_synonym) {
  };
  bool operator==(const QueryDeclaration &other) const override;
  bool operator==(const QueryDeclaration *other) const override;
  QuerySynonym *getSynonym() const;
  std::string toString() const override;
};


// Synonym declaration of "stmt x;" where x is the synonym
class StatementDeclaration : public SynonymDeclaration {
 private:
  QuerySynonym *query_synonym_;
 public:
  explicit StatementDeclaration(QuerySynonym *query_synonym)
      : SynonymDeclaration(query_synonym, EntityType::kStatement), query_synonym_(query_synonym) {
  };
};

// Synonym declaration of "stmt x;" where x is the synonym
class IfDeclaration : public SynonymDeclaration {
 private:
  QuerySynonym *query_synonym_;
 public:
  explicit IfDeclaration(QuerySynonym *query_synonym)
      : SynonymDeclaration(query_synonym, EntityType::kIfStmt), query_synonym_(query_synonym) {
  };
};

// Synonym declaration of "stmt x;" where x is the synonym
class ReadDeclaration : public SynonymDeclaration {
 private:
  QuerySynonym *query_synonym_;
 public:
  explicit ReadDeclaration(QuerySynonym *query_synonym)
      : SynonymDeclaration(query_synonym, EntityType::kIfStmt), query_synonym_(query_synonym) {
  };
};

// Synonym declaration of "stmt x;" where x is the synonym
class CallDeclaration : public SynonymDeclaration {
 private:
  QuerySynonym *query_synonym_;
 public:
  explicit CallDeclaration(QuerySynonym *query_synonym)
      : SynonymDeclaration(query_synonym, EntityType::kIfStmt), query_synonym_(query_synonym) {
  };
};

// Synonym declaration of "stmt x;" where x is the synonym
class PrintDeclaration : public SynonymDeclaration {
 private:
  QuerySynonym *query_synonym_;
 public:
  explicit PrintDeclaration(QuerySynonym *query_synonym)
      : SynonymDeclaration(query_synonym, EntityType::kIfStmt), query_synonym_(query_synonym) {
  };
};

// Synonym declaration of "stmt x;" where x is the synonym
class WhileDeclaration : public SynonymDeclaration {
 private:
  QuerySynonym *query_synonym_;
 public:
  explicit WhileDeclaration(QuerySynonym *query_synonym)
      : SynonymDeclaration(query_synonym, EntityType::kIfStmt), query_synonym_(query_synonym) {
  };
};

// Synonym declaration of "stmt x;" where x is the synonym
class AssignDeclaration : public SynonymDeclaration {
 private:
  QuerySynonym *query_synonym_;
 public:
  explicit AssignDeclaration(QuerySynonym *query_synonym)
      : SynonymDeclaration(query_synonym, EntityType::kIfStmt), query_synonym_(query_synonym) {
  };
};

// Synonym declaration of "stmt x;" where x is the synonym
class VariableDeclaration : public SynonymDeclaration {
 private:
  QuerySynonym *query_synonym_;
 public:
  explicit VariableDeclaration(QuerySynonym *query_synonym)
      : SynonymDeclaration(query_synonym, EntityType::kIfStmt), query_synonym_(query_synonym) {
  };
};
// Synonym declaration of "stmt x;" where x is the synonym
class ConstantDeclaration : public SynonymDeclaration {
 private:
  QuerySynonym *query_synonym_;
 public:
  explicit ConstantDeclaration(QuerySynonym *query_synonym)
      : SynonymDeclaration(query_synonym, EntityType::kIfStmt), query_synonym_(query_synonym) {
  };
};
// Synonym declaration of "stmt x;" where x is the synonym
class ProcedureDeclaration : public SynonymDeclaration {
 private:
  QuerySynonym *query_synonym_;
 public:
  explicit ProcedureDeclaration(QuerySynonym *query_synonym)
      : SynonymDeclaration(query_synonym, EntityType::kIfStmt), query_synonym_(query_synonym) {
  };
};





















// Static declaration of "stmt x;" where x is the synonym
class StaticDeclaration : public QueryDeclaration {
 private:
  std::string value_;
 public:
  explicit StaticDeclaration(EntityType entity_type, std::string value)
      : QueryDeclaration(DeclarationType::kStatic, entity_type), value_(std::move(value)) {
  }
  bool operator==(const QueryDeclaration &other) const override;
  bool operator==(const QueryDeclaration *other) const override;
  std::string toString() const override;
};

// Static declaration of  Wildcard "_"
class WildcardDeclaration : public StaticDeclaration {
 private:
  std::unordered_set<EntityType> wildcard_types_;
 public:
  explicit WildcardDeclaration(std::unordered_set<EntityType> wildcard_types = {})
      : StaticDeclaration(EntityType::kWildcard, "_"), wildcard_types_(std::move(wildcard_types)) {
  };
  void setWildcardType(std::unordered_set<EntityType> types);
  std::unordered_set<EntityType> getWildcardType();
};


// Static declaration of Expression "(x + (y * z))"
class ExpressionDeclaration : public StaticDeclaration {
 public:
  explicit ExpressionDeclaration(std::string string) : StaticDeclaration(EntityType::kExpression, std::move(string)) {
  }
};

// Static declaration of Expression _"(x + (y * z))"_
class WildCardExpressionDeclaration : public StaticDeclaration {
 public:
  explicit WildCardExpressionDeclaration(std::string string)
      : StaticDeclaration(EntityType::kWildcardExpression, std::move(string)) {
  }
};

// Static declaration of string eg. Modifies("x", v)
class IdentDeclaration : public StaticDeclaration {
 public:
  explicit IdentDeclaration(std::string string) : StaticDeclaration(EntityType::kIdent, std::move(string)) {
  }
};

// Static declaration of integer eg. Modifies(3, v)
class IntegerDeclaration : public StaticDeclaration {
 public:
  explicit IntegerDeclaration(std::string number) : StaticDeclaration(EntityType::kInteger, std::move(number)) {
  }
};

struct QueryDeclarationHashFunction {
  size_t operator()(const QueryDeclaration &declaration) const {
    if (declaration.getDeclarationType() == DeclarationType::kSynonym) {
      return QuerySynonymHashFunction().operator()(static_cast<const SynonymDeclaration &>(declaration).getSynonym());
    }
    return QuerySynonymHashFunction().operator()(QuerySynonym::empty());
  }
  size_t operator()(const QueryDeclaration *declaration) const {
    if (declaration->getDeclarationType() == DeclarationType::kSynonym) {
      return QuerySynonymHashFunction().operator()(static_cast<const SynonymDeclaration *>(declaration)->getSynonym());
    }
    return QuerySynonymHashFunction().operator()(QuerySynonym::empty());
  }
};

struct QueryDeclarationPointerEquality {
  bool operator()(const QueryDeclaration *lhs, const QueryDeclaration *rhs) const {
    if (lhs->getEntityType() == EntityType::kWildcard || rhs->getEntityType() == EntityType::kWildcard) { return true; }
    return (*lhs) == (*rhs);
  }
};
