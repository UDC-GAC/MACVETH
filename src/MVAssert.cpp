#include "include/MVAssert.h"

using namespace macveth;

// ---------------------------------------------
void macveth::MVAssertSkip(bool cond, std::string Msg, jmp_buf Goto, int Val) {
  if (!cond) {
    std::cerr << BOLDMAGENTA << "[MACVETH SKIPPING] " << RESET << Msg
              << std::endl;
    longjmp(Goto, Val);
  }
}

// ---------------------------------------------
void macveth::MVWarn(std::string Msg) {
  std::cerr << BOLDYELLOW << "[MACVETH WARNING] " << RESET << Msg << std::endl;
}

// ---------------------------------------------
void macveth::MVInfo(std::string Msg) {
  std::cerr << BOLDBLUE << "[MACVETH INFO] " << RESET << Msg << std::endl;
}

// ---------------------------------------------
void macveth::MVAssert(bool cond, const char *Msg) {
  if (!cond) {
    std::cerr << BOLDRED << "[MACVETH FATAL ERROR] " << RESET << Msg
              << std::endl;
    exit(1);
  }
}

// ---------------------------------------------
void macveth::MVErr(std::string Msg) {
  std::cerr << BOLDRED << "[MACVETH FATAL ERROR] " << RESET << Msg << std::endl;
  exit(1);
}