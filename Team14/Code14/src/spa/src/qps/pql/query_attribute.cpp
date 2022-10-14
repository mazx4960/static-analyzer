#include "query_attribute.h"

std::string QueryAttribute::toString() const {
  return "toString is not implemented";
}
bool QueryAttribute::operator==(const QueryAttribute &other) const {
  return other.toString() == this->toString();
}