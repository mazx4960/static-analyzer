#include "query_attribute.h"

std::string QueryAttribute::toString() const {
  return this->attribute_name_;
}
bool QueryAttribute::operator==(const QueryAttribute &other) const {
  return other.toString() == this->toString();
}