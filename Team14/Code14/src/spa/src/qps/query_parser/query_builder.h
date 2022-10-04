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

 public:
  explicit QueryBuilder();
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
  SelectCall *buildSelectCall(QueryDeclaration *synonym_declaration, std::vector<QueryClause *> clause_vector);
  QueryCall *getQueryCall();
  PatternClause *buildAssignPattern(QueryDeclaration *syn_assign,
                                    QueryDeclaration *ent_ref,
                                    QueryDeclaration *expression_spec);
  static SuchThatClause *buildSuchThat(RsType type, QueryDeclaration *first, QueryDeclaration *second);
  QueryDeclaration *buildWildcardExpression(std::string expression = "");
  QueryDeclaration *buildExpression(std::string expression);
};
