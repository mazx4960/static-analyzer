#include "sp.h"

#include "simple_lexer.h"
#include "simple_parser.h"
#include "extractor/extractor.h"

void SP::SetPKB(PKB *pkb) {
  this->pkb_ = pkb;
}
void SP::LoadSource(std::ifstream &source_stream) {
  SimpleLexer lexer(&source_stream);
  auto tokens = lexer.lex();
  SimpleParser parser(tokens);
  auto *program_node = parser.Parse();

  std::vector<Entity *> entities = EntityExtractor::Extract(program_node);
  std::vector<Relationship *> relationships = RelationshipExtractor::Extract(program_node);
  this->pkb_->Save(entities);
}
