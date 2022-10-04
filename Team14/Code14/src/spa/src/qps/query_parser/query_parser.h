// Copyright 2022 CS3203 Team14. All rights reserved.

#include <unordered_set>
#include <utility>
#include <vector>
#include <map>

#include "commons/lexer/token.h"
#include "commons/parser/parser_exceptions.h"
#include "qps/pql/query.h"
#include "qps/pql/query_declaration.h"
#include "query_builder.h"

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
  void parseQueryCall();
  QueryCall *getQueryCall();
  void parseDeclaration();
  QueryDeclaration *getDeclaration(Token *synonym);
  QuerySynonym *parseSynonym();
  void parseClause();
  void parsePattern();
  void parseSuchThat();
  void parseFollows();
  void parseParent();
  void parseUses();
  void parseModifies();
  QueryDeclaration *parseExpression();
  IdentDeclaration *parseQuotedDeclaration();
  QueryDeclaration *parseStmtRefDeclaration();
  QueryDeclaration *parseEntRefDeclaration();
  IntegerDeclaration *parseLiteralDeclaration();
  IdentDeclaration *parseIdentDeclaration();
  QueryDeclaration *parseWildcard(EntityType type);
  QueryDeclaration *parseAnyRefDeclaration();
  std::string parseFlattenedExpression();
  static void expect(Token *token, const std::unordered_set<TokenType> &expected_types);
  void parseCalls();
  void parseNext();
  void parseAffects();
  QueryDeclaration *parseProcedureRefDeclaration();
};
