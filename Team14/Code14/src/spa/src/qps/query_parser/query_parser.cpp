// Copyright 2022 CS3203 Team14. All rights reserved.

#include "commons/lexer/token.h"
#include "query_parser.h"
#include "query_builder.h"
#include "query_lexer.h"
#include "query_validator/query_validator.h"
#include "spdlog/spdlog.h"

Query QueryParser::parse(std::ifstream *query_stream) {
  QueryLexer lexer = QueryLexer(query_stream);
  QueryValidator validator = QueryValidator(lexer.lex());
  validator.validate();
  spdlog::info("Tokens validated");
  QueryBuilder builder = QueryBuilder();
  builder.withDeclarations(validator.getDeclarations());
  builder.withQueryCalls(validator.getQueryCalls());
  return builder.build();
}
