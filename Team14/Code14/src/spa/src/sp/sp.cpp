#include "sp.h"

#include <spdlog/spdlog.h>

#include "extractor/extractor.h"
#include "simple_lexer.h"
#include "simple_parser.h"

void SP::SetPKB(IPKBSaver *pkb) { this->pkb_ = pkb; }
void SP::LoadSource(std::ifstream &source_stream) {
  SimpleLexer lexer(&source_stream);
  auto tokens = lexer.lex();
  auto *program_node = SimpleParser::ParseProgram(tokens);

  std::vector<Entity *> entities = EntityExtractor::Extract(program_node);
  spdlog::info("Extracted {} entities", entities.size());
  for (auto *entity : entities) { spdlog::debug("{}", entity->ToString()); }
  this->pkb_->saveEntities(entities);

  std::vector<Relationship *> relationships = RelationshipExtractor::Extract(program_node);
  spdlog::info("Extracted {} relationships", relationships.size());
  for (auto *relationship : relationships) { spdlog::debug("{}", relationship->ToString()); }
}
