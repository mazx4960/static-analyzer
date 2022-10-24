// Copyright 2022 CS3203 Team14. All rights reserved.

#pragma once

#include <string>

class QuerySynonym {
 private:
  std::string synonym_;

 public:
  explicit QuerySynonym(std::string synonym) : synonym_(std::move(synonym)) {
  };
  static QuerySynonym *empty();

  [[nodiscard]] std::string toString() const;
  bool operator==(const QuerySynonym &) const;
  bool operator!=(const QuerySynonym &) const;
};

struct QuerySynonymHashFunction {
  size_t operator()(const QuerySynonym &synonym) const {
    return std::hash<std::string>()(synonym.toString());
  }
  size_t operator()(const QuerySynonym *synonym) const {
    return std::hash<std::string>()(synonym->toString());
  };
};

struct QuerySynonymPointerEquality {
  bool operator()(const QuerySynonym *lhs, const QuerySynonym *rhs) const {
    return lhs->toString() == rhs->toString();
  }
};

struct QuerySynonymPointerComparison {
  bool operator()(const QuerySynonym *lhs, const QuerySynonym *rhs) const {
    return lhs->toString().compare(rhs->toString()) < 0;
  }
};

