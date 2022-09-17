// Copyright 2022 CS3203 Team14. All rights reserved.
#include "pattern_manager.h"

#include <spdlog/spdlog.h>

PatternManager::PatternManager() { this->pattern_table_ = new PatternTable(); }
PatternTable *PatternManager::GetTable() { return this->pattern_table_; }
void PatternManager::Populate(const std::vector<Pattern *> &patterns) {
  for (Pattern *pattern : patterns) { this->pattern_table_->Populate(*pattern); }
}
EntityPointerUnorderedSet PatternManager::Get(Entity *variable, const std::string &expr) {
  spdlog::debug("Retrieving all statements that matches {} = {}", variable->GetValue(), expr);
  auto matches = EntityPointerUnorderedSet();
  auto set = this->pattern_table_->Get(variable);
  for (const auto &pair : set) {
    auto full_expr = pair.second;
    if (full_expr.find(expr) != std::string::npos) { matches.insert(pair.first); }
  }
  std::string result_string;
  for (auto *match : matches) { result_string += match->GetValue() + ", "; }
  spdlog::debug("Results[{}]: {}", matches.size(), result_string);
  return matches;
}
