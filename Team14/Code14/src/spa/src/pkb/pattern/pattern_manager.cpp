// Copyright 2022 CS3203 Team14. All rights reserved.
#include "pattern_manager.h"

PatternManager::PatternManager() { this->pattern_table_ = new PatternTable(); }
PatternTable* PatternManager::GetTable() { return this->pattern_table_; }
void PatternManager::Populate(const std::vector<Pattern*>& patterns) {
  for (Pattern* pattern : patterns) { this->pattern_table_->Populate(*pattern); }
}
std::unordered_set<Entity*, EntityHashFunction, EntityPointerEquality> PatternManager::Get(Entity* variable,
                                                                                           const std::string& expr) {
  auto matches = std::unordered_set<Entity*, EntityHashFunction, EntityPointerEquality>();
  auto set = this->pattern_table_->Get(variable);
  for (const auto& pair : set) {
    auto full_expr = pair.second;
    if (full_expr.find(expr) != std::string::npos) { matches.insert(pair.first); }
  }
  return matches;
}
