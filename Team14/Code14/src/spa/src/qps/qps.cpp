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
  std::string token_string;
  for (auto *token : tokens) {
    token_string += token->ToString() + " ";
  }
  spdlog::debug("Tokens[{}]: ", tokens.size(), token_string);

  Query *query;
  try {
    spdlog::info("Parsing tokens...");
    QueryParser parser(tokens);
    query = parser.parse();
    spdlog::info("Tokens parsed");
    spdlog::debug("QueryBlueprint: {}", query->toString());
    spdlog::info("Building Query...");
    QueryBuilder builder = QueryBuilder(query);
    query = builder.build();
    spdlog::info("Query built");
    spdlog::debug("Query: {}", query->toString());
  } catch (ParseSemanticError &e) {
    spdlog::info("Building failed");
    spdlog::debug("SemanticError: {}", e.what());
    return Result::semanticError();
  } catch (ParseSyntaxError &e) {
    spdlog::info("Parsing failed");
    spdlog::debug("SyntaxError: {}", e.what());
    return Result::syntacticError();
  }
  Result *result = (new QueryEvaluator(this->pkb_, *query))->evaluate();
  return result;
}
