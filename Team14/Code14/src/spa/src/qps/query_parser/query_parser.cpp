// Copyright 2022 CS3203 Team14. All rights reserved.

#include "commons/lexer/token.h"
#include "query_parser.h"
#include "query_builder.h"
#include "query_lexer.h"

Query QueryParser::parse(std::ifstream *query_stream) {
  QueryLexer lexer = QueryLexer(query_stream);
  QueryBuilder builder = QueryBuilder(lexer.lex());
  return QueryBuilder::build();
}