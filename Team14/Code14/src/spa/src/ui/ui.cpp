// Copyright 2022 CS3203 Team14. All rights reserved.

#include "ui.h"

#include <utility>

#include "commons/reader.h"
#include "spdlog/spdlog.h"

UI::UI(std::string source_file, const std::string &query_file, int mode = 0)
    : source_file_(std::move(source_file)), query_stream_(StreamReader::GetStreamFromFile(query_file)) {
  // Setup logging
  if (mode == 2) {
    spdlog::set_level(spdlog::level::debug);
  } else if (mode == 1) {
    spdlog::set_level(spdlog::level::info);
  } else {
    spdlog::set_level(spdlog::level::off);
  }
}
UI::UI() {
  spdlog::set_level(spdlog::level::off);
}
void UI::SetSP(SP *sp) {
  this->sp_ = sp;
}
void UI::SetQPS(QPS *qps) {
  this->qps_ = qps;
}
void UI::SetSourceFile(std::string source_file) {
  this->source_file_ = std::move(source_file);
}
void UI::SetQueryFile(const std::string &query_file) {
  this->query_stream_ = StreamReader::GetStreamFromFile(query_file);
}
void UI::SetQueryString(const std::string &query_string) {
  this->query_stream_ = StreamReader::GetStreamFromString(query_string);
}
void UI::Run() {
  if (this->sp_ == nullptr || this->qps_ == nullptr) {
    spdlog::error("SP or QPS not found! Exiting program...");
    return;
  }
  LoadSource();
  auto *result = ExecuteQuery();
  DisplayResults(result);
}
void UI::LoadSource() {
  if (this->source_file_.empty()) {
    spdlog::error("Source file not found! Exiting program...");
    return;
  }
  if (this->sp_ == nullptr) {
    spdlog::error("SP not found! Exiting program...");
    return;
  }
  spdlog::info("Reading source file...");
  std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
  std::istream *source_stream = StreamReader::GetStreamFromFile(this->source_file_);
  this->sp_->LoadSource(source_stream);
  std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
  spdlog::info("Source file loaded.");
  spdlog::info("Time taken to load source file: {} ms",
               std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count());
}
Result *UI::ExecuteQuery() {
  if (this->query_stream_ == nullptr) {
    spdlog::error("Query file not found! Exiting program...");
    return Result::Empty();
  }
  if (this->qps_ == nullptr) {
    spdlog::error("QPS not found! Exiting program...");
    return Result::Empty();
  }
  spdlog::info("Reading query file...");
  std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
  Result *result = this->qps_->EvaluateQuery(query_stream_);
  std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
  spdlog::info("Query executed.");
  spdlog::info("Time taken to execute query:  {} ms",
               std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count());
  return result;
}
void UI::DisplayResults(const Result *result) {
  spdlog::info("====================BEGIN QUERY RESULTS====================");
  spdlog::info(result->ToString());
  spdlog::info("=====================END QUERY RESULTS=====================");
}
