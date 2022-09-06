// Copyright 2022 CS3203 Team14. All rights reserved.

#pragma once

#include "ui/ui.h"

class SPA {
 private:
  UI *ui_;

 public:
  SPA(std::string source_file, std::string query_file, bool debug_mode);
  ~SPA() = default;
  void Run();
};
