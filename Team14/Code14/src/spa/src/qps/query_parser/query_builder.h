// Copyright 2022 CS3203 Team14. All rights reserved.

#include <unordered_set>
#include <utility>
#include <vector>

#include "qps/exceptions.h"
#include "commons/lexer/token.h"
#include "commons/parser/parser_exceptions.h"
#include "qps/pql/query.h"
#include "blueprint.h"

class QueryBuilder {
 private:
  std::unordered_map<std::string, QuerySynonym *> synonym_table_;
  std::vector<DeclarationBlueprint *> declaration_bps_;
  SelectBlueprint *select_bp_;
  std::vector<ClauseBlueprint *> clauses_bp_;

  QuerySynonym *GetSynonym(const std::string &name);
  bool IsSynonymDeclared(const std::string &name);

 public:
  explicit QueryBuilder() = default;
  Query *Build();

  void AddDeclaration(DeclarationBlueprint *declaration);
  void SetSelect(SelectBlueprint *select);
  void AddClauses(std::vector<ClauseBlueprint *> &clauses);
  std::unordered_set<QuerySynonym *> BuildDeclarations();
  SelectCall *BuildSelect();
  std::vector<QueryClause *> BuildClauses();
  PatternClause *BuildPattern(PatternBlueprint *clause_blueprint);
  SuchThatClause *BuildSuchThat(SuchThatBlueprint *clause_blueprint);
  WithClause *BuildWith(WithBlueprint *clause_blueprint);
  std::vector<ElemReference *> BuildElems(const std::vector<ElemBlueprint *> &elem_blueprints);
  QueryReference *BuildReference(BaseBlueprint *blueprint);
};
