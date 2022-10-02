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

  using SuchThatFunction = SuchThatClause *(*)(Token *);
  std::map<RsType, std::function<string(*)>> such_that_mapping_ {
    {RsType::kFollows, &QueryParser::parseFollows},
    {RsType::kParent, &QueryParser::parseParent},
    {RsType::kUses, &QueryParser::parseUses},
    {RsType::kModifies, &QueryParser::parseModifies}};

  using PatternFunction = PatternClause *(QueryParser::*)(Token *);
  std::map<RsType, PatternFunction> pattern_mapping_ {
      {RsType::kAssignPattern, &QueryParser::parsePattern}};

  Token *nextToken();
  Token *peekToken();
  bool outOfTokens();

 public:
  explicit QueryParser(std::vector<Token *> tokens, QueryBuilder builder = QueryBuilder());
  Query parse();
  std::vector<QueryDeclaration *> getDeclarations();
  std::vector<QueryCall *> getQueryCalls();
  void parseDeclarations();
  QueryCall *parseQueryCall(Token *call);
  void parseDeclaration(Token *prefix);
  QueryDeclaration *getDeclaration(Token *synonym);
  QueryDeclaration *getStmtDeclaration(Token *synonym);
  QueryDeclaration *getEntDeclaration(Token *synonym);
  QuerySynonym *parseSynonym(Token *synonym);
  QueryClause *parseClause(Token *clause);
  PatternClause *parsePattern(Token *synonym);
  SuchThatClause *parseSuchThat(Token *relationship);
  SuchThatClause *parseFollows(Token *star);
  SuchThatClause *parseParent(Token *star);
  SuchThatClause *parseUses(Token *star);
  SuchThatClause *parseModifies(Token *star);
  QueryDeclaration *parseExpression();
  StringDeclaration *parseQuotedDeclaration();
  QueryDeclaration *parseStmtRefDeclaration(Token *stmtref);
  QueryDeclaration *parseEntRefDeclaration(Token *entref);
  IntegerDeclaration *parseLiteralDeclaration(Token *literal);
  StringDeclaration *parseStringDeclaration(Token *symbol);
  QueryDeclaration *parseWildcard(EntityType type, Token *wildcard);
  void parseBracket(Token *bracket, bool open);
  void parseComma(Token *comma);
  QueryDeclaration *parseAnyRefDeclaration(Token *ref);
  std::string parseFlattenedExpression(Token *);
  void parseQuote(Token *quote);
};
