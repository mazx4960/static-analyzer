// Copyright 2022 CS3203 Team14. All rights reserved.

#include <unordered_set>
#include <utility>
#include <vector>

#include "commons/lexer/token.h"
#include "commons/parser/parser_exceptions.h"
#include "qps/pql/query.h"

class QueryBuilder {
 private:
  std::vector<QueryDeclaration *> query_declarations_;

  std::unordered_set<std::string> synonyms_;

  SelectCall *query_call_;


 public:
  explicit QueryBuilder();
  Query build();
  QuerySynonym *buildSynonym(const std::string &synonym);
  void *buildDeclaration(EntityType type, QuerySynonym *synonym);
  QueryDeclaration *getDeclaration(const std::string &synonym);
  bool isDeclared(const std::string &synonym);

  WildCardStmtDeclaration *buildWildcardStmt();
  IntegerDeclaration *buildLiteral(const std::string &number);
  StringDeclaration *buildString(const std::string &str);
  WildCardEntDeclaration * buildWildcardEnt();
  SelectCall *buildSelectCall(QueryDeclaration *synonym_declaration, std::vector<QueryClause *> clause_vector);
  PatternClause *buildAssignPattern(QueryDeclaration *pattern_synonym,
                                    QueryDeclaration *first_param,
                                    QueryDeclaration *second_param);
  SuchThatClause *buildSuchThat(RsType type, QueryDeclaration *first, QueryDeclaration *second);
  QueryDeclaration *buildWildcardExpression(std::string expression);
  QueryDeclaration *buildExpression(std::string expression);
  QueryDeclaration *getStmtDeclaration(const std::string& synonym);
  QueryDeclaration *getEntDeclaration(const std::string& synonym);
};
