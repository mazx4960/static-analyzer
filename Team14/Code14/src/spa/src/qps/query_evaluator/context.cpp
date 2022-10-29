// Copyright 2022 CS3203 Team14. All rights reserved.

#include <spdlog/spdlog.h>
#include "context.h"

EntitySet Context::Get(QuerySynonym *synonym) const {
  auto it = context_.find(synonym);
  if (it == context_.end()) {
    return EntitySet();
  }
  return it->second;
}

void Context::Set(QuerySynonym *synonym, EntitySet &entities) {
  if (context_.find(synonym) != context_.end()) {
    context_.erase(synonym);
  }
  context_[synonym] = entities;
}
void Context::LogStatistics() {
  spdlog::info("==================== Context STATISTICS ====================");
  int total = 0;
  for (auto &pair : context_) {
    total += pair.second.size();
    spdlog::info("Synonym {} has {} entities", pair.first->GetName(), pair.second.size());
  }
  spdlog::info("Total context size: {} entities", total);
  spdlog::info("============================================================");
}
