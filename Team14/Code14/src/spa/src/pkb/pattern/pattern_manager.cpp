// Copyright 2022 CS3203 Team14. All rights reserved.
#include "pattern_manager.h"

PatternManager::PatternManager() { this->pattern_table_ = new PatternTable(); }
PatternTable* PatternManager::GetTable() { return this->pattern_table_; }
void PatternManager::Populate(const std::vector<Pattern*>& patterns) {
  for (Pattern* pattern : patterns) { this->pattern_table_->Populate(*pattern); }
}
std::unordered_set<Entity*, EntityHashFunction, EntityPointerEquality> PatternManager::Get() { return nullptr; }
