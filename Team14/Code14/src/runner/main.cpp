#include <iostream>
#include <cstring>
#include "spa.h"

int main(int argc, char *argv[]) {
  bool flag = false;
  std::string source_file;
  std::string query_file;
  if (argc == 3) {
    source_file = argv[1];
    query_file = argv[2];
  } else if (argc == 4) {
    if (strcmp(argv[1], "-v") == 0) {
      flag = true;
    }
    source_file = argv[2];
    query_file = argv[3];
  } else {
    std::cout << "Invalid number of arguments" << std::endl;
    std::cout << "Usage: [-v]" << argv[0] << " <source file> <query file> ";
    return 1;
  }

  // Initialize and run SPA
  auto *spa = new SPA(source_file, query_file, flag);
  spa->Run();
}
