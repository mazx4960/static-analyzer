// Copyright 2022 CS3203 Team14. All rights reserved.
#include "qps.h"
#include "spdlog/spdlog.h"

void QPS::SetPKB(IPKBQuerier *pkb) {
  this->pkb_ = pkb;
}
Result *QPS::EvaluateQuery(std::istream *query_stream) {
  QueryLexer lexer(query_stream);
  auto tokens = lexer.lex();
  spdlog::info("Generated query tokens");
  std::string token_string = "Tokens: ";
  for (auto *token : tokens) {
    token_string += token->ToString() + " ";
  }
  spdlog::debug(token_string);

  Query *query;
  spdlog::info("Parsing tokens...");
  QueryParser parser(tokens);
  query = parser.Parse();
  spdlog::info("Query built");
  spdlog::debug("Query: {}", query->toString());

  QueryEvaluator qe(this->pkb_, *query);
  Result *result = qe.evaluate();
  return result;
}
