// Copyright 2022 CS3203 Team14. All rights reserved.

#include <unordered_set>
#include <utility>
#include <vector>
#include <map>
#include <functional>

#include "qps/pql/query_keywords.h"
#include "commons/lexer/token.h"
#include "commons/parser/parser_exceptions.h"
#include "qps/pql/query.h"
#include "qps/pql/query_reference.h"
#include "query_builder.h"

class QueryParser {
 private:
  std::vector<Token *> tokens_;
  Declarations declarations_;
  Clauses clauses_;
  int token_index_ = 0;

  Token *nextToken();
  Token *peekToken();
  bool outOfTokens();

  void parseDeclarationStatement();
  SynonymReference *parseDeclaration(EntityType type);
  static SuchThatClause *parseSuchThat(RsType rs_type, QueryReference *first, QueryReference *second);
  IdentReference * parseQuotedReference();
  SynonymReference *parseSynonymReference();
  WildcardReference *parseWildcardReference();
  IntegerReference *parseIntegerReference();
  IdentReference *parseIdentReference();
  std::string parseFlattenedExpression();
 public:
  explicit QueryParser(std::vector<Token *> tokens);
  Query *parse();
  Declarations parseDeclarations();
  QuerySynonym *parseSynonym();
  QueryCall *parseQueryCall();
  Clauses parseClauses();
  QueryClause *parseClause();
  SuchThatClause *parseSuchThat();
  QueryReference *parseReference();
  PatternClause *parsePattern();
  ExpressionSpec *parseExpression();

  static void expect(Token *token, const std::unordered_set<TokenType> &expected_types);
  QueryClause *getPreviousClause();
};