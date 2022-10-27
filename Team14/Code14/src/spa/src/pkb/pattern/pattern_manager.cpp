// Copyright 2022 CS3203 Team14. All rights reserved.
#include "pattern_manager.h"

#include <spdlog/spdlog.h>

PatternManager::PatternManager() {
  this->pattern_table_ = new PatternTable();
}
PatternTable *PatternManager::GetTable() {
  return this->pattern_table_;
}
void PatternManager::Populate(const std::vector<Pattern *> &patterns) {
  for (Pattern *pattern : patterns) {
    this->pattern_table_->Populate(*pattern);
  }
}
EntityPointerUnorderedSet PatternManager::Get(Entity *variable, const std::string &expr, bool isSubmatch) {
  this->num_queries_++;
  spdlog::debug("Retrieving all statements that matches {} = {}", variable->GetValue(), expr);
  auto matches = EntityPointerUnorderedSet();
  auto set = this->pattern_table_->Get(variable);
  for (const auto &pair : set) {
    auto full_expr = pair.second;// RHS of the statement
    if (isSubmatch) {
      if (expr.empty() || full_expr.find(expr) != std::string::npos) {
        matches.insert(pair.first);
      }
    } else if (full_expr == expr) {
      matches.insert(pair.first);
    }
  }
  std::string result_string;
  for (auto *match : matches) {
    result_string += match->GetValue() + ", ";
  }
  spdlog::debug("Results[{}]: {}", matches.size(), result_string);
  return matches;
}
void PatternManager::LogStatistics() {
  spdlog::info("Number of queries: {}", this->num_queries_);
  spdlog::info("Number of patterns: {}", this->pattern_table_->Size());
}
