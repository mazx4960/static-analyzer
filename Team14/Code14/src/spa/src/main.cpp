#include "spa.h"
#include "spdlog/spdlog.h"
#include "ui/ui.h"

int main(int argc, char *argv[]) {
  if (argc != 2) {
    spdlog::error("Usage: {} <source file> <query file>", argv[0]);
    return 1;
  }

  // Setup logging
  spdlog::set_level(spdlog::level::debug);// Set global log level to debug
  spdlog::info("Initialising SPA...");

  // Initialize and run SPA
  auto *spa = new SPA(argv[1], argv[2]);
  spa->Run();
}
