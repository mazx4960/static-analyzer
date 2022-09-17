// Copyright 2022 CS3203 Team14. All rights reserved.
#pragma once

#include <istream>
#include <string>

#include "commons/result.h"
#include "qps/qps.h"
#include "sp/sp.h"

class UI {
 private:
  SP *sp_;

  QPS *qps_;

  std::string source_file_;

  std::istream *query_stream_;

 public:
  explicit UI(std::string source_file, const std::string &query_file, int mode);
  explicit UI();
  void SetSP(SP *sp);
  void SetQPS(QPS *qps);
  void SetSourceFile(std::string source_file);
  void SetQueryFile(const std::string &query_file);
  void SetQueryString(const std::string &query_string);
  void Run();
  void LoadSource();
  Result *ExecuteQuery();
  static void DisplayResults(const Result *result);
};
