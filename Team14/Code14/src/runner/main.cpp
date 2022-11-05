#include <cstring>
#include <iostream>

#include "spa.h"

int main(int argc, char *argv[]) {
  int verbosity = 1;
  std::string source_file;
  std::string query_file;
  if (argc == 3) {
    source_file = argv[1];
    query_file = argv[2];
  } else if (argc == 4) {
    if (strcmp(argv[1], "-v") == 0) { verbosity = 2; }
    source_file = argv[2];
    query_file = argv[3];
  } else {
    std::cout << "Invalid number of arguments" << std::endl;
    std::cout << "Usage: [-v]" << argv[0] << " <source file> <query file> ";
    return 1;
  }

  // Initialize and run SPA
  auto *spa = new SPA(source_file, query_file, verbosity);
  spa->Run();
}
