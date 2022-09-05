// Copyright 2022 CS3203 Team14. All rights reserved.

#include "spa.h"

#include <utility>

SPA::SPA(std::string source_file, std::string query_file) {
  // Initialize all components.
  this->ui_ = new UI(std::move(source_file), std::move(query_file));
  auto *sp = new SP();
  auto *qps = new QPS();
  auto *pkb = new PKB();

  // Set up the component dependencies.
  this->ui_->SetSP(sp);
  this->ui_->SetQPS(qps);
  sp->SetPKB(pkb);
  qps->SetPKB(pkb);
}
void SPA::Run() {
  this->ui_->Run();
}
