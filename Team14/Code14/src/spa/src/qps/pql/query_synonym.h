// Copyright 2022 CS3203 Team14. All rights reserved.

#pragma once

#include <string>

class QuerySynonym {
 private:
  std::string synonym_;

 public:
  explicit QuerySynonym(std::string);

  [[nodiscard]] std::string toString() const;
  bool operator==(const QuerySynonym &) const;
};

class QuerySynonymHashFunction {
 public:
  size_t operator()(const QuerySynonym &) const;
  size_t operator()(const QuerySynonym *) const;
};


