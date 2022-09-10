// Copyright 2022 CS3203 Team14. All rights reserved.

#include "ui.h"

#include <utility>

#include "commons/reader.h"
#include "qps/query_parser/query_parser.h"
#include "spdlog/spdlog.h"

UI::UI(std::string source_file, std::string query_file)
    : source_file_(std::move(source_file)),
      query_file_(std::move(query_file)) {}
void UI::SetSP(SP *sp) { this->sp_ = sp; }
void UI::SetQPS(QPS *qps) { this->qps_ = qps; }
void UI::Run() {
  if (this->sp_ == nullptr || this->qps_ == nullptr) {
    spdlog::error("SP or QPS not found! Exiting program...");
    return;
  }
  LoadSource();
  Result result = ExecuteQuery();
  DisplayResults(result);
}
void UI::LoadSource() {
  spdlog::info("Reading source file...");
  std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
  std::ifstream source_stream = StreamReader::GetStreamFromFile(this->source_file_);
  this->sp_->LoadSource(source_stream);
  std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
  spdlog::info("Source file loaded.");
  spdlog::info("Time taken to load source file: {} ms",
               std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count());
}
Result UI::ExecuteQuery() {
  spdlog::info("Reading query file...");
  std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
  std::ifstream query_stream = StreamReader::GetStreamFromFile(this->query_file_);

  Query parsed_query = QueryParser::parse(&query_stream);
  Result result = this->qps_->evaluate(parsed_query);
  std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
  spdlog::info("Query executed.");
  spdlog::info("Time taken to execute query:  {} ms",
               std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count());
  return result;
}
void UI::DisplayResults(const Result &result) {
  spdlog::info("Sorting results...");
  std::vector<std::string> results_list = result.get_sorted_results_list();

  spdlog::info("====================BEGIN QUERY RESULTS====================");
  std::string result_string = result.get_synonym().getSynonym() + ": ";
  for (const std::string &s : results_list) { result_string += s + " "; }
  spdlog::info("{}", result_string);
  spdlog::info("====================END QUERY RESULTS====================");
}
