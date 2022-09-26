// Copyright 2022 CS3203 Team14. All rights reserved.

#pragma once

#include <stdexcept>
#include <string>

class PKBException : public std::runtime_error {
 public:
  explicit PKBException(const std::string &message) : std::runtime_error("PKB exception: " + message) {
  }
};
