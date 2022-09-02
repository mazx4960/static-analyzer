// Copyright 2022 CS3203 Team14. All rights reserved.

#include "query_token.h"

bool QueryToken::operator==(const QueryToken &other) const {
  return this->type == other.type && this->value == other.value;
}

