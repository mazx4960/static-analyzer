// Copyright 2022 CS3203 Team14. All rights reserved.

#include <unordered_set>
#include <utility>
#include <vector>
#include <map>

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

 public:
  explicit QueryParser(std::vector<Token *> tokens);
  Query *parse();
  static void expect(Token *token, const std::unordered_set<TokenType> &expected_types);

  Declarations parseDeclarations();
  QueryCall *parseQueryCall();
  Clauses parseClauses();
  QueryReference *parseReference();
  IdentReference * parseQuotedReference();
  SynonymReference *parseSynonymReference();
  WildcardReference *parseWildcardReference();
  IntegerReference *parseIntegerReference();
  QuerySynonym *parseSynonym();
  IdentReference *parseIdentReference();
  QueryClause *parseClause();
  SuchThatClause *parseSuchThat();
  static SuchThatClause *parseSuchThat(RsType rs_type, QueryReference *first, QueryReference *second);
  PatternClause *parsePattern();
  ExpressionSpec *parseExpression();
  std::string parseFlattenedExpression();
  SynonymReference *parseDeclaration(EntityType type);
};
