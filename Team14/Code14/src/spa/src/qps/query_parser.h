// Copyright 2022 CS3203 Team14. All rights reserved.

#include <unordered_set>
#include <utility>
#include <vector>

#include "commons/lexer/token.h"
#include "commons/parser/parser_exceptions.h"
#include "qps/pql/query.h"
#include "qps/pql/query_declaration.h"

class QueryParser {
 private:
  std::vector<Token *> tokens_;
  std::vector<QueryDeclaration *> query_declarations_;

  std::unordered_set<std::string> synonyms_;

  std::vector<QueryCall *> query_calls_;

  int token_index_ = 0;

  Token *nextToken();
  Token *peekToken();
  bool outOfTokens();

 public:
  explicit QueryParser(std::vector<Token *> tokens);
  void parse();
  std::vector<QueryDeclaration *> getDeclarations();
  std::vector<QueryCall *> getQueryCalls();
  void parseDeclarations();
  void parseQueryCalls();
  QueryDeclaration *parseDeclaration();
  QueryDeclaration *getDeclaration(const std::string &synonym);
  QuerySynonym *parseSynonym();
  QueryClause *parseClause();
  PatternClause *parsePattern();
  SuchThatClause *parseSuchThat();
  SuchThatClause *parseFollows();
  SuchThatClause *parseParent();
  SuchThatClause *parseUses();
  SuchThatClause *parseModifies();
  bool isDeclared(const std::string &synonym);
  QueryDeclaration *parseExpression();
  StringDeclaration *parseQuotedDeclaration();
  QueryDeclaration *parseStmtRefDeclaration(bool allowWild);
  QueryDeclaration *parseEntRefDeclaration(bool allowWild);
  IntegerDeclaration *parseLiteralDeclaration();
  StringDeclaration *parseStringDeclaration();
};
