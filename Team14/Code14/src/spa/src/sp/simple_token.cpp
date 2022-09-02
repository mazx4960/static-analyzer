// Copyright 2022 CS3203 Team14. All rights reserved.

#include "simple_token.h"

bool SimpleToken::operator==(const SimpleToken &other) const {
  return this->type == other.type && this->value == other.value;
}

