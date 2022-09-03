#include "synonym.h"

#include <utility>

Synonym::Synonym(std::string synonym) {
  this->synonym = std::move(synonym);
}

bool Synonym::operator==(const Synonym& other) const {
  return (synonym == other.synonym);
}
Synonym::Synonym() {}
