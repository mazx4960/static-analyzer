// Copyright 2022 CS3203 Team14. All rights reserved.

#pragma once

#include <string>
#include <utility>

class Token {
 public:
  explicit Token(std::string value);
  std::string value;
  bool operator==(const Token &other) const;
};

