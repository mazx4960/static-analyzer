#include "sp.h"

#include <spdlog/spdlog.h>

#include "simple_lexer.h"
#include "simple_parser.h"
#include "sp/extractor/entity_extractor.h"
#include "sp/extractor/pattern_extractor.h"
#include "sp/extractor/relationship_extractor.h"

void SP::SetPKB(IPKBPopulator *pkb) { this->pkb_ = pkb; }
void SP::LoadSource(std::ifstream &source_stream) {
  SimpleLexer lexer(&source_stream);
  auto tokens = lexer.lex();
  spdlog::info("Generated source tokens");
  std::string token_string = "Tokens: ";
  for (auto *token : tokens) { token_string += token->ToString() + " "; }
  spdlog::debug(token_string);

  auto *program_node = SimpleParser::ParseProgram(tokens);
  spdlog::info("Converted tokens to abstract syntax tree");

  std::vector<Entity *> entities = EntityExtractor::ExtractAll(program_node);
  spdlog::info("Extracted {} entities", entities.size());
  for (auto *entity : entities) { spdlog::debug("{}", entity->ToString()); }
  this->pkb_->populate(entities);

  std::vector<Relationship *> relationships = RelationshipExtractor::ExtractAll(program_node);
  spdlog::info("Extracted {} relationships", relationships.size());
  for (auto *relationship : relationships) { spdlog::debug("{}", relationship->ToString()); }

  std::vector<Pattern *> patterns = PatternExtractor::ExtractAll(program_node);
  spdlog::info("Extracted {} patterns", patterns.size());

}
