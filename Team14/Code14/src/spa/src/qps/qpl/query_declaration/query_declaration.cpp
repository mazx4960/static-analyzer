#include "query_declaration.h"
#include <utility>

QueryDeclaration::QueryDeclaration(std::string entity, Synonym synonym) {
  this->entity = std::move(entity);
  this->synonym = std::move(synonym);
}
