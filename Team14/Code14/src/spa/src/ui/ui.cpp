// Copyright 2022 CS3203 Team14. All rights reserved.

#include "ui.h"

#include <utility>

#include "commons/reader.h"
#include "spdlog/spdlog.h"

UI::UI(std::string source_file, std::string query_file)
    : source_file_(std::move(source_file)),
      query_file_(std::move(query_file)) {}
void UI::SetSP(SP* sp) {
  this->sp_ = sp;
}
void UI::SetQPS(QPS* qps) {
  this->qps_ = qps;
}
void UI::Run() {
  if (this->sp_ == nullptr || this->qps_ == nullptr) {
    spdlog::error("SP or QPS not found! Exiting program...");
    return;
  }
  spdlog::info("Reading source file...");
  std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
  std::ifstream source_stream(this->source_file_);
  this->sp_->LoadSource(source_stream);
  std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
  spdlog::info("Source file loaded.");
  spdlog::info("Time taken to load source file: {} ms", std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count());

  spdlog::info("Reading query file...");
  begin = std::chrono::steady_clock::now();
  std::ifstream query_stream(this->query_file_);
  // TODO: execute query
  end = std::chrono::steady_clock::now();
  spdlog::info("Query executed.");
  spdlog::info("Time taken to execute query: {} ms", std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count());

  // TODO: print results
}
