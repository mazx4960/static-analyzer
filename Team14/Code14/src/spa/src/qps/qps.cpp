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
  QueryBuilder builder = QueryBuilder();
  QueryParser parser(tokens, builder);
  spdlog::info("Parsing tokens...");
  Query *query;
  try {
    query = parser.parse();
  } catch (ParseSemanticError &e) {
    return Result::semanticError();
  } catch (ParseSyntaxError &e) {
    return Result::syntacticError();
  }

  std::string declaration_string;
  for (auto *declaration : query->getDeclarations()) {
    declaration_string += EntityTypeToString(declaration->getType()) + ":" + declaration->toString() + " ";
  }
  spdlog::debug("Declarations[{}]: {}", query->getDeclarations().size(), declaration_string);
  spdlog::info("Query parsed");
  Result *result = (new QueryEvaluator(this->pkb_, *query))->evaluate();
  return result;
}
