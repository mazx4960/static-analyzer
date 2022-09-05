#include "sp.h"

#include "simple_lexer.h"

void SP::SetPKB(PKB *pkb) {
  this->pkb_ = pkb;
}
void SP::LoadSource(std::ifstream &source_stream) {
  SimpleLexer lexer(&source_stream);
  auto tokens = lexer.lex();
  // TODO: parse tokens
  // TODO: extract entities
  // TODO: extract relationships
  // TODO: populate pkb
}
