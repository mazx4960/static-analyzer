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
#include "query_syntax_rules.h"

class QueryParser {
 private:
  std::vector<Token *> tokens_;
  std::vector<SynonymDeclaration *> maybe_declarations_;
  Query *maybe_query_;
  std::vector<QueryClause *> maybe_clauses_;

  int token_index_ = 0;

  Token *nextToken();
  Token *peekToken();
  bool outOfTokens();
  void parseDeclaration();
  void checkReferenceSyntax(SyntaxRuleType syntax_type);

 public:
  explicit QueryParser(std::vector<Token *> tokens);
  Query *parse();

  QueryCall *parseQueryCall();

  QuerySynonym *parseSynonym();
  std::vector<SynonymDeclaration *> parseDeclarations();
  QueryClause *parseClause();
  PatternClause *parsePattern();
  SuchThatClause *parseSuchThat();

  QueryDeclaration *parseReference(SyntaxRuleType syntax_type);
  IdentDeclaration *parseQuotedDeclaration();
  IntegerDeclaration *parseLiteralDeclaration();
  IdentDeclaration *parseIdentDeclaration();
  WildcardDeclaration *parseWildcard();
  StaticDeclaration *parseExpression();
  std::string parseFlattenedExpression();
  SynonymDeclaration *parseInlineSynonymDeclaration();

  static void expect(Token *token, const std::unordered_set<TokenType> &expected_types);
};
