// Copyright 2022 CS3203 Team14. All rights reserved.
#include "qps.h"

#include "spdlog/spdlog.h"
#include "query_builder.h"

void QPS::SetPKB(IPKBQuerier *pkb) { this->pkb_ = pkb; }
Result *QPS::EvaluateQuery(std::ifstream &query_stream) {
  QueryLexer lexer(&query_stream);
  auto tokens = lexer.lex();
  spdlog::info("Generated query tokens");
  std::string token_string = "Tokens: ";
  for (auto *token : tokens) { token_string += token->ToString() + " "; }
  spdlog::debug(token_string);

  QueryParser validator(tokens);
  validator.parse();
  spdlog::info("Tokens validated");
  QueryBuilder builder = QueryBuilder();
  builder.withDeclarations(validator.getDeclarations());
  builder.withQueryCalls(validator.getQueryCalls());
  Query query = builder.build();

  Result *result = (new QueryEvaluator(this->pkb_, query))->evaluate();
  return result;
}
