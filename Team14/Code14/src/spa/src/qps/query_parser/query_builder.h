// Copyright 2022 CS3203 Team14. All rights reserved.

#include <unordered_set>
#include <utility>
#include <vector>

#include "commons/lexer/token.h"
#include "commons/parser/parser_exceptions.h"
#include "qps/pql/query.h"
#include "query_rules.h"

class QueryBuilder {
 private:
  std::vector<QueryDeclaration *> query_declarations_;

  std::unordered_set<std::string> synonyms_;

  SelectCall *query_call_;
  QueryDeclaration *selected_declaration_;
  std::vector<QueryClause *> unchecked_clauses_;
  std::vector<QueryClause *> built_clauses_;

 public:
  QueryBuilder();
  Query *build();
  QuerySynonym *buildSynonym(const std::string &synonym);
  QueryDeclaration *buildDeclaration(EntityType type, QuerySynonym *synonym);
  QueryDeclaration *getDeclaration(const std::string &synonym);
  std::vector<QueryDeclaration *> getDeclarations();
  bool isDeclared(const std::string &synonym);

  static WildCardStmtDeclaration *buildWildcardStmt();
  static IntegerDeclaration *buildLiteral(const std::string &number);
  IdentDeclaration *buildIdent(const std::string &str);
  static WildCardEntDeclaration * buildWildcardEnt();
  void withSelectCall(QueryDeclaration *synonym_declaration);
  QueryCall *getQueryCall();
  PatternClause *buildAssignPattern(PatternClause *clause);
  static SuchThatClause *buildSuchThat(SuchThatClause *clause);
  QueryDeclaration *buildWildcardExpression(std::string expression = "");
  QueryDeclaration *buildExpression(std::string expression);
  void withAssignPattern(QueryDeclaration *syn_assign, QueryDeclaration *ent_ref, QueryDeclaration *expression_spec);
  void withSuchThat(RsType type, QueryDeclaration *first, QueryDeclaration *second);
};
