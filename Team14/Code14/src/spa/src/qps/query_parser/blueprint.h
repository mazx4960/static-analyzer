// Copyright 2022 CS3203 Team14. All rights reserved.

#pragma once

#include <string>
#include "qps/pql/types.h"
#include "qps/pql/query_clause.h"
#include "rules.h"

/**
 * Represents an atomic value in a PQL query.
 */
class BaseBlueprint {
 private:
  ReferenceType reference_type_;
  std::string value_;

 public:
  BaseBlueprint(ReferenceType reference_type, std::string value)
      : reference_type_(reference_type), value_(std::move(value)) {
  };
  [[nodiscard]] ReferenceType getReferenceType() const {
    return reference_type_;
  };
  [[nodiscard]] std::string getValue() const {
    return value_;
  };
  [[nodiscard]] std::string toString() const {
    return RefTypeToString(reference_type_) + ": " + value_;
  };
};

class SynonymBlueprint : public BaseBlueprint {
 public:
  explicit SynonymBlueprint(std::string value) : BaseBlueprint(ReferenceType::kSynonym, std::move(value)) {
  };
};

class ExprBlueprint : public BaseBlueprint {
 private:
  bool is_exact_;
 public:
  explicit ExprBlueprint(std::string value, bool is_exact) : BaseBlueprint(ReferenceType::kIdent, std::move(value)),
                                                             is_exact_(is_exact) {
  };
  [[nodiscard]] bool isExact() const {
    return is_exact_;
  };
};

class ElemBlueprint : public BaseBlueprint {
 private:
  AttributeType attribute_type_;
 public:
  explicit ElemBlueprint(SynonymBlueprint *synonym_blueprint, AttributeType attribute_type) : BaseBlueprint(
      ReferenceType::kAttr,
      synonym_blueprint->getValue()), attribute_type_(attribute_type) {
  };
  [[nodiscard]] AttributeType getAttributeType() const {
    return this->attribute_type_;
  };
};

/**
 * Represents a declaration in a PQL query.
 */
class DeclarationBlueprint {
 private:
  SynonymBlueprint *synonym_;
  EntityType entity_type_;
 public:
  explicit DeclarationBlueprint(SynonymBlueprint *synonym, EntityType entity_type)
      : synonym_(synonym), entity_type_(entity_type) {
  };
  [[nodiscard]] std::string getName() const {
    return synonym_->getValue();
  };
  [[nodiscard]] EntityType getEntityType() const {
    return entity_type_;
  };
};

/**
 * Represents a Select clause in a PQL query.
 */
class SelectBlueprint {
 private:
  SelectType select_type_;
  std::vector<ElemBlueprint *> blueprint_references_;
 public:
  explicit SelectBlueprint(SelectType select_type, std::vector<ElemBlueprint *> blueprint_references) : select_type_(
      select_type), blueprint_references_(std::move(blueprint_references)) {
  };
  [[nodiscard]] SelectType getSelectType() const {
    return select_type_;
  };
  [[nodiscard]] std::vector<ElemBlueprint *> getBlueprintReferences() const {
    return blueprint_references_;
  };
};

/**
 * Represents a clause in a PQL query.
 */
class ClauseBlueprint {
 private:
  ClauseType clause_type_;

 public:
  explicit ClauseBlueprint(ClauseType clause_type) : clause_type_(clause_type) {
  };
  [[nodiscard]] ClauseType getClauseType() const {
    return this->clause_type_;
  };
  [[nodiscard]] virtual std::string toString() const = 0;
};

class SuchThatBlueprint : public ClauseBlueprint {
 private:
  RsType rs_type_;
  BaseBlueprint *first_;
  BaseBlueprint *second_;
 public:
  SuchThatBlueprint(RsType rs_type, BaseBlueprint *first, BaseBlueprint *second)
      : ClauseBlueprint(ClauseType::kSuchThat), rs_type_(rs_type), first_(first), second_(second) {
  };
  [[nodiscard]] RsType getRsType() const {
    return this->rs_type_;
  };
  [[nodiscard]] BaseBlueprint *getFirst() const {
    return this->first_;
  };
  [[nodiscard]] BaseBlueprint *getSecond() const {
    return this->second_;
  };
  bool checkSyntax() {
    return CheckSuchThat(this->rs_type_, this->first_->getReferenceType(), this->second_->getReferenceType());
  }
  [[nodiscard]] std::string toString() const override {
    return "SuchThatBlueprint: " + RsTypeToString(rs_type_) + " " + first_->toString() + " " + second_->toString();
  };
};

/**
 * Represents a pattern clause in a PQL query.
 */
class PatternBlueprint : public ClauseBlueprint {
 private:
  SynonymBlueprint *stmt_;
  BaseBlueprint *var_;
  ExprBlueprint *expr_;
  ExprBlueprint *expr_2_;
 public:
  PatternBlueprint(SynonymBlueprint *stmt, BaseBlueprint *var, ExprBlueprint *expr)
      : ClauseBlueprint(ClauseType::kPattern), stmt_(stmt), var_(var), expr_(expr) {
  };
  PatternBlueprint(SynonymBlueprint *stmt, BaseBlueprint *var, ExprBlueprint *expr, ExprBlueprint *expr_2)
      : ClauseBlueprint(ClauseType::kPattern), stmt_(stmt), var_(var), expr_(expr), expr_2_(expr_2) {
  };
  [[nodiscard]] SynonymBlueprint *getStmt() const {
    return this->stmt_;
  };
  [[nodiscard]] BaseBlueprint *getVar() const {
    return this->var_;
  };
  [[nodiscard]] ExprBlueprint *getExpr() const {
    return this->expr_;
  };
  [[nodiscard]] ExprBlueprint *getExpr2() const {
    return this->expr_2_;
  };
  bool checkSyntax() {
    return CheckPattern(this->var_->getReferenceType());
  }
  [[nodiscard]] std::string toString() const override {
    return "PatternBlueprint: " + stmt_->toString() + " " + var_->toString() + " " + expr_->toString();
  };
};

/**
 * Represents a with clause in a PQL query.
 */
class WithBlueprint : public ClauseBlueprint {
 private:
  Comparator comparator_;
  BaseBlueprint *first_;
  BaseBlueprint *second_;
 public:
  WithBlueprint(Comparator comparator, BaseBlueprint *first, BaseBlueprint *second)
      : ClauseBlueprint(ClauseType::kWith), comparator_(comparator), first_(first), second_(second) {
  };
  [[nodiscard]] Comparator getComparator() const {
    return this->comparator_;
  };
  [[nodiscard]] BaseBlueprint *getFirst() const {
    return this->first_;
  };
  [[nodiscard]] BaseBlueprint *getSecond() const {
    return this->second_;
  };
  bool checkSyntax() {
    return CheckWith(this->first_->getReferenceType(), this->second_->getReferenceType());
  }
  [[nodiscard]] std::string toString() const override {
    return "WithBlueprint: " + first_->toString() + " " + second_->toString();
  };
};
