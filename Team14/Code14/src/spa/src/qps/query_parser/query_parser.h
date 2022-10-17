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

using TokenIterator = std::vector<Token *>::iterator;

class QueryParser {
 private:
  std::vector<Token *> tokens_;
  TokenIterator token_stream_;
  QueryBuilder *query_builder_;

  bool ShouldStop();
  Token *Expect(const Token &);
  Token *Expect(TokenType);

 public:
  explicit QueryParser(std::vector<Token *> tokens);
  Query *Parse();
  void ParseQuery();
  void ParseDeclaration();
  void ParseSelect();
  void ParseClause(std::vector<ClauseBlueprint *> &clauses);
  void ParseSuchThat(std::vector<ClauseBlueprint *> &clauses);
  void ParsePattern(std::vector<ClauseBlueprint *> &clauses);
  void ParseWith(std::vector<ClauseBlueprint *> &clauses);
  BaseBlueprint *ParseBase();
  SynonymBlueprint *ParseSynonym();
  ElemBlueprint *ParseElem();
  ExprBlueprint *ParseExpr();
};
