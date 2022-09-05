// Copyright 2022 CS3203 Team14. All rights reserved.

#pragma once

#include <string>

class QuerySynonym {
 private:
  std::string synonym_;

 public:
  explicit QuerySynonym(std::string synonym);

  [[nodiscard]] std::string getSynonym() const;
  bool operator==(const QuerySynonym &other) const;
};


