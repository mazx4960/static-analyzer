// Copyright 2022 CS3203 Team14. All rights reserved.

#pragma once

#include <string>

#include "qps/pql/types.h"
#include "qps/pql/query_clause.h"
#include "rules.h"

/**
 * Represents an atomic value in a PQL query.
 */
class ReferenceBlueprint {
 private:
  ReferenceType reference_type_;

  std::string value_;

 public:
  ReferenceBlueprint(ReferenceType reference_type, std::string value)
      : reference_type_(reference_type), value_(std::move(value)) {
  };
  [[nodiscard]] ReferenceType getReferenceType() const;
  [[nodiscard]] std::string getValue() const;
  [[nodiscard]] std::string toString() const;
  virtual bool operator==(const ReferenceBlueprint &other) const;
};

class ExprBlueprint : public ReferenceBlueprint {
 private:
  bool is_exact_;

 public:
  explicit ExprBlueprint(std::string value, bool is_exact) : ReferenceBlueprint(ReferenceType::kIdent, std::move(value)),
                                                             is_exact_(is_exact) {
  };
  [[nodiscard]] bool isExact() const;
  bool operator==(const ReferenceBlueprint &other) const override;
};

class SynonymBlueprint : public ReferenceBlueprint {
 public:
  explicit SynonymBlueprint(std::string value) : ReferenceBlueprint(ReferenceType::kSynonym, std::move(value)) {
  };
  bool operator==(const ReferenceBlueprint &other) const override;
};

class ElemBlueprint : public ReferenceBlueprint {
 private:
  AttributeType attribute_type_;

 public:
  explicit ElemBlueprint(SynonymBlueprint *synonym_blueprint, AttributeType attribute_type) : ReferenceBlueprint(
      ReferenceType::kAttr,
      synonym_blueprint->getValue()), attribute_type_(attribute_type) {
  };
  [[nodiscard]] AttributeType getAttributeType() const;
  bool operator==(const ReferenceBlueprint &other) const override;
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
  [[nodiscard]] std::string getName() const;
  [[nodiscard]] EntityType getEntityType() const;
  bool operator==(const DeclarationBlueprint &other) const;
};

/**
 * Represents a Select clause in a PQL query.
 */
class SelectBlueprint {
 private:
  std::vector<ElemBlueprint *> blueprint_references_;
  ElemBlueprint *single_reference_;

 public:
  explicit SelectBlueprint(std::vector<ElemBlueprint *> blueprint_references)
      : single_reference_(nullptr), blueprint_references_(std::move(blueprint_references)) {
  };
  explicit SelectBlueprint(ElemBlueprint *single_reference)
      : single_reference_(single_reference), blueprint_references_({single_reference}) {
  };
  [[nodiscard]] std::vector<ElemBlueprint *> getBlueprintReferences() const;
  [[nodiscard]] ElemBlueprint *getSingleReference() const;
  bool operator==(const SelectBlueprint &other) const;
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
  virtual bool operator==(const ClauseBlueprint &other) const = 0;
};

class SuchThatBlueprint : public ClauseBlueprint {
 private:
  RsType rs_type_;

  ReferenceBlueprint *first_;

  ReferenceBlueprint *second_;

 public:
  SuchThatBlueprint(RsType rs_type, ReferenceBlueprint *first, ReferenceBlueprint *second)
      : ClauseBlueprint(ClauseType::kSuchThat), rs_type_(rs_type), first_(first), second_(second) {
  };
  [[nodiscard]] RsType getRsType() const;
  [[nodiscard]] ReferenceBlueprint *getFirst() const;
  [[nodiscard]] ReferenceBlueprint *getSecond() const;
  [[nodiscard]] bool checkSyntax() const;
  [[nodiscard]] std::string toString() const override;
  bool operator==(const ClauseBlueprint &other) const override;
};

/**
 * Represents a pattern clause in a PQL query.
 */
class PatternBlueprint : public ClauseBlueprint {
 private:
  SynonymBlueprint *stmt_;

  ReferenceBlueprint *var_;

  ExprBlueprint *expr_;

  ExprBlueprint *expr_2_;

 public:
  PatternBlueprint(SynonymBlueprint *stmt, ReferenceBlueprint *var, ExprBlueprint *expr)
      : ClauseBlueprint(ClauseType::kPattern), stmt_(stmt), var_(var), expr_(expr), expr_2_(nullptr) {
  };
  PatternBlueprint(SynonymBlueprint *stmt, ReferenceBlueprint *var, ExprBlueprint *expr, ExprBlueprint *expr_2)
      : ClauseBlueprint(ClauseType::kPattern), stmt_(stmt), var_(var), expr_(expr), expr_2_(expr_2) {
  };
  [[nodiscard]] SynonymBlueprint *getStmt() const;
  [[nodiscard]] ReferenceBlueprint *getVar() const;
  [[nodiscard]] ExprBlueprint *getExpr() const;
  [[nodiscard]] ExprBlueprint *getExpr2() const;
  [[nodiscard]] bool checkSyntax() const;
  [[nodiscard]] std::string toString() const override;
  bool operator==(const ClauseBlueprint &other) const override;
};

/**
 * Represents a with clause in a PQL query.
 */
class WithBlueprint : public ClauseBlueprint {
 private:
  Comparator comparator_;

  ReferenceBlueprint *first_;

  ReferenceBlueprint *second_;

 public:
  WithBlueprint(Comparator comparator, ReferenceBlueprint *first, ReferenceBlueprint *second)
      : ClauseBlueprint(ClauseType::kWith), comparator_(comparator), first_(first), second_(second) {
  };
  [[nodiscard]] Comparator getComparator() const;
  [[nodiscard]] ReferenceBlueprint *getFirst() const;
  [[nodiscard]] ReferenceBlueprint *getSecond() const;
  [[nodiscard]] bool checkSyntax() const;
  [[nodiscard]] std::string toString() const override;
  bool operator==(const ClauseBlueprint &other) const override;
};
