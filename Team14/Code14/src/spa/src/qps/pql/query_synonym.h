// Copyright 2022 CS3203 Team14. All rights reserved.

#pragma once

#include <string>
#include "commons/types.h"

class QuerySynonym {
 private:
  std::string name_;
  EntityType type_;
  int usage_;

 public:
  explicit QuerySynonym(std::string name, EntityType type) : name_(std::move(name)), type_(type) {
  };

  [[nodiscard]] std::string GetName() const;
  [[nodiscard]] EntityType GetEntityType() const;
  [[nodiscard]] int GetUsage() const;
  [[nodiscard]] std::string ToString() const;
  void IncrementUsage();
  bool operator==(const QuerySynonym &) const;
  bool operator!=(const QuerySynonym &) const;
};

struct QuerySynonymHashFunction {
  size_t operator()(const QuerySynonym &synonym) const {
    return std::hash<std::string>()(synonym.ToString());
  }
  size_t operator()(const QuerySynonym *synonym) const {
    return std::hash<std::string>()(synonym->ToString());
  };
};

struct QuerySynonymPointerEquality {
  bool operator()(const QuerySynonym *lhs, const QuerySynonym *rhs) const {
    return lhs->ToString() == rhs->ToString();
  }
};

struct QuerySynonymPointerComparison {
  bool operator()(const QuerySynonym *lhs, const QuerySynonym *rhs) const {
    return lhs->ToString().compare(rhs->ToString()) < 0;
  }
};

