#include "sp.h"

#include "simple_lexer.h"
#include "simple_parser.h"
#include "sp/extractor/entity_extractor.h"
#include "sp/extractor/pattern_extractor.h"
#include "sp/extractor/relationship_extractor.h"
#include "spdlog/spdlog.h"

void SP::SetPKB(IPKBPopulator *pkb) { this->pkb_ = pkb; }
void SP::LoadSource(std::istream *source_stream) {
  SimpleLexer lexer(source_stream);
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

  std::vector<Relationship *> relationships = RelationshipExtractor::ExtractAll(program_node);
  spdlog::info("Extracted {} relationships", relationships.size());
  for (auto *relationship : relationships) { spdlog::debug("{}", relationship->ToString()); }

  std::vector<Pattern *> patterns = PatternExtractor::ExtractAll(program_node);
  spdlog::info("Extracted {} patterns", patterns.size());
  for (auto *pattern : patterns) { spdlog::debug("{}", pattern->ToString()); }

  spdlog::info("Populating PKB...");
  spdlog::info("Populating Entities...");
  this->pkb_->populate(entities);
  spdlog::info("Populating Relationships...");
  this->pkb_->populate(relationships);
  spdlog::info("Populating Patterns...");
  this->pkb_->populate(patterns);
  spdlog::info("PKB Populated.");
}
