// Copyright 2022 CS3203 Team14. All rights reserved.
#pragma once

#include <fstream>
#include <string>

#include "qps/qps.h"
#include "sp/sp.h"

class UI {
 private:
  SP* sp_;
  QPS* qps_;
  std::string source_file_;
  std::string query_file_;

 public:
  explicit UI(std::string source_file, std::string query_file);
  void SetSP(SP* sp);
  void SetQPS(QPS* qps);
  void Run();
};
