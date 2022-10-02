// Copyright 2022 CS3203 Team14. All rights reserved.

#include <unordered_set>
#include <utility>
#include <vector>
#include <map>

#include "commons/lexer/token.h"
#include "commons/parser/parser_exceptions.h"
#include "qps/pql/query.h"
#include "qps/pql/query_declaration.h"
#include "qps/query_builder.h"

class QueryParser {
 private:
  QueryBuilder builder_;
  std::vector<Token *> tokens_;
  int token_index_ = 0;

 public:
  Token *nextToken();
  Token *peekToken();
  bool outOfTokens();
  explicit QueryParser(std::vector<Token *> tokens, QueryBuilder builder = QueryBuilder());
  Query *parse();
  void parseDeclarations();
  std::vector<QueryDeclaration *> getDeclarations();
  QueryCall *parseQueryCall();
  QueryCall *getQueryCall();
  void parseDeclaration();
  QueryDeclaration *getDeclaration(Token *synonym);
  QueryDeclaration *getStmtDeclaration(Token *synonym);
  QueryDeclaration *getEntDeclaration(Token *synonym);
  QuerySynonym *parseSynonym();
  QueryClause *parseClause();
  PatternClause *parsePattern();
  SuchThatClause *parseSuchThat();
  SuchThatClause *parseFollows();
  SuchThatClause *parseParent();
  SuchThatClause *parseUses();
  SuchThatClause *parseModifies();
  QueryDeclaration *parseExpression();
  StringDeclaration *parseQuotedDeclaration();
  QueryDeclaration *parseStmtRefDeclaration();
  QueryDeclaration *parseEntRefDeclaration();
  IntegerDeclaration *parseLiteralDeclaration();
  StringDeclaration *parseStringDeclaration();
  QueryDeclaration *parseWildcard(EntityType type);
  void parseBracket(Token *bracket, bool open);
  void parseComma(Token *comma);
  QueryDeclaration *parseAnyRefDeclaration();
  std::string parseFlattenedExpression();
  void parseQuote(Token *quote);
};
