// Copyright 2022 CS3203 Team14. All rights reserved.

#pragma once

#include <string>

class QuerySynonym {
 public:
  explicit QuerySynonym(std::string synonym);
  std::string synonym_;
  [[nodiscard]] std::string getSynonym() const;
};


