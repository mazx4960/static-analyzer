// Copyright 2022 CS3203 Team14. All rights reserved.

#include "query_builder.h"
#include "qps/pql/query_keywords.h"

SynonymReference *QueryBuilder::GetSynonymReference(const std::string &name) {
  if (synonym_table_.find(name) == synonym_table_.end()) {
    throw ParseSemanticError("Synonym " + name + " not declared");
  }
  return synonym_table_[name];
}

Query *QueryBuilder::Build() {
  auto built_declarations = BuildDeclarations();
  auto *built_select = BuildSelect();
  auto built_clauses = BuildClauses();
  return new Query(built_declarations, built_select, built_clauses);
}

void QueryBuilder::AddDeclaration(DeclarationBlueprint *declaration) {
  if (declaration == nullptr) {
    throw BuilderError("No declaration given");
  }
  declaration_bps_.push_back(declaration);
}

void QueryBuilder::AddSelect(SelectBlueprint *select) {
  if (select == nullptr) {
    throw BuilderError("No select given");
  }
  select_bp_ = select;
}

void QueryBuilder::AddClauses(std::vector<ClauseBlueprint *> &clauses) {
  clauses_bp_.insert(clauses_bp_.end(), clauses.begin(), clauses.end());
}

std::vector<SynonymReference *> QueryBuilder::BuildDeclarations() {
  auto declarations = std::vector<SynonymReference *>();
  for (auto *declaration : declaration_bps_) {
    std::string name = declaration->getName();
    if (synonym_table_.find(name) != synonym_table_.end()) {
      throw ParseSemanticError("Duplicate synonym declaration: " + name);
    }
    auto *synonym = new QuerySynonym(name);
    auto *synonym_ref = new SynonymReference(synonym, declaration->getEntityType());
    synonym_table_[name] = synonym_ref;
    declarations.push_back(synonym_ref);
  }
  return declarations;
}
SelectCall *QueryBuilder::BuildSelect() {
  if (select_bp_ == nullptr) {
    throw ParseSemanticError("Missing Select call");
  }
  switch (select_bp_->getSelectType()) {
    case SelectType::kBoolean:return new BooleanSelect();
    case SelectType::kElem:return new ElemSelect(BuildElems(select_bp_->getBlueprintReferences()));
    default:throw ParseSemanticError("Invalid Select type");
  }
}
std::vector<QueryClause *> QueryBuilder::BuildClauses() {
  auto clauses = std::vector<QueryClause *>();
  for (auto *clause : clauses_bp_) {
    QueryClause *built_clause;
    switch (clause->getClauseType()) {
      case ClauseType::kPattern: {
        built_clause = BuildPattern(static_cast<PatternBlueprint *>(clause));
        break;
      }
      case ClauseType::kSuchThat: {
        built_clause = BuildSuchThat(static_cast<SuchThatBlueprint *>(clause));
        break;
      }
      case ClauseType::kWith: {
        built_clause = BuildWith(static_cast<WithBlueprint *>(clause));
        break;
      }
      default:throw ParseSemanticError("Unsupported clause type!");
    }
    clauses.push_back(built_clause);
  }
  return clauses;
}

std::vector<ElemReference *> QueryBuilder::BuildElems(const std::vector<ElemBlueprint *> &elem_blueprints) {
  auto elems = std::vector<ElemReference *>();
  for (auto *elem_blueprint : elem_blueprints) {
    std::string name = elem_blueprint->getValue();
    auto *synonym = GetSynonymReference(name);
    auto *attr_ref = new AttrReference(synonym, elem_blueprint->getAttributeType());
    if (!attr_ref->isSemanticallyCorrect()) {
      throw ParseSemanticError("Invalid AttrRef");
    }
    elems.push_back(attr_ref);
  }
  return elems;
}

QueryReference *QueryBuilder::BuildReference(BaseBlueprint *blueprint) {
  QueryReference *ref;
  switch (blueprint->getReferenceType()) {
    case ReferenceType::kSynonym:ref = GetSynonymReference(blueprint->getValue());
      break;
    case ReferenceType::kIdent:ref = new IdentReference(blueprint->getValue());
      break;
    case ReferenceType::kInteger:ref = new IntegerReference(blueprint->getValue());
      break;
    case ReferenceType::kWildcard:ref = new WildcardReference();
      break;
    case ReferenceType::kAttr:
      ref = new AttrReference(GetSynonymReference(blueprint->getValue()),
                              static_cast<ElemBlueprint *>(blueprint)->getAttributeType());
      break;
    default:throw ParseSemanticError("Invalid reference type for bp: " + blueprint->toString());
  }
  return ref;
}

PatternClause *QueryBuilder::BuildPattern(PatternBlueprint *clause_blueprint) {
  auto *stmt = GetSynonymReference(clause_blueprint->getStmt()->getValue());
  auto *var_bp = clause_blueprint->getVar();
  if (var_bp == nullptr) {
    throw ParseSemanticError("Invalid variable in Pattern clause");
  }
  QueryReference *var = BuildReference(var_bp);
  ExpressionSpec *expr_spec;
  auto *expr_bp = clause_blueprint->getExpr();
  if (expr_bp == nullptr) {
    throw ParseSemanticError("Invalid expression in Pattern clause");
  }
  if (expr_bp->isExact()) {
    expr_spec = new ExactExpression(expr_bp->getValue());
  } else {
    expr_spec = new WildExpression(expr_bp->getValue());
  }
  auto *clause = new PatternClause(stmt, var, expr_spec);
  if (!clause->isSemanticallyCorrect()) {
    throw ParseSemanticError("Invalid pattern parameter type");
  }
  return clause;
}

SuchThatClause *QueryBuilder::BuildSuchThat(SuchThatBlueprint *clause_blueprint) {
  auto *first_bp = clause_blueprint->getFirst();
  if (first_bp == nullptr) {
    throw ParseSemanticError("Invalid first parameter in SuchThat clause");
  }
  QueryReference *first = BuildReference(first_bp);
  auto *second_bp = clause_blueprint->getSecond();
  if (second_bp == nullptr) {
    throw ParseSemanticError("Invalid second parameter in SuchThat clause");
  }
  QueryReference *second = BuildReference(second_bp);
  auto *clause = SuchThatClause::Create(clause_blueprint->getRsType(), first, second);

  if (!clause->isSemanticallyCorrect()) {
    throw ParseSemanticError("Invalid such that parameter type");
  }
  return clause;
}

WithClause *QueryBuilder::BuildWith(WithBlueprint *clause_blueprint) {
  auto *first_bp = clause_blueprint->getFirst();
  if (first_bp == nullptr) {
    throw ParseSemanticError("Invalid first parameter in with clause");
  }
  QueryReference *first = BuildReference(first_bp);
  auto *second_bp = clause_blueprint->getSecond();
  if (second_bp == nullptr) {
    throw ParseSemanticError("Invalid second parameter in with clause");
  }
  QueryReference *second = BuildReference(second_bp);
  auto *clause = new WithClause(clause_blueprint->getComparator(), first, second);

  if (!clause->isSemanticallyCorrect()) {
    throw ParseSemanticError("Invalid with parameter type");
  }
  return clause;
}

