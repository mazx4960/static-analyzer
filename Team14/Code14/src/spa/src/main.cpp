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

  // Initialize all components.
  auto *ui = new UI(argv[1], argv[2]);
  auto *sp = new SP();
  auto *qps = new QPS();
  auto *pkb = new PKB();

  // Set up the component dependencies.
  ui->SetSP(sp);
  ui->SetQPS(qps);
  sp->SetPKB(pkb);
  qps->SetPKB(pkb);

  // Lets go!
  ui->Run();
}
