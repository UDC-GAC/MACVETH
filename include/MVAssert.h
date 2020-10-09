#ifndef MACVETH_ASSERT_H
#define MACVETH_ASSERT_H

#include <iostream>
#include <setjmp.h>
#include <string>

#ifndef WIN32
// the following are UBUNTU/LINUX, and MacOS ONLY terminal color codes.
#define RESET "\033[0m"
#define BLACK "\033[30m"              /* Black */
#define RED "\033[31m"                /* Red */
#define GREEN "\033[32m"              /* Green */
#define YELLOW "\033[33m"             /* Yellow */
#define BLUE "\033[34m"               /* Blue */
#define MAGENTA "\033[35m"            /* Magenta */
#define CYAN "\033[36m"               /* Cyan */
#define WHITE "\033[37m"              /* White */
#define BOLDBLACK "\033[1m\033[30m"   /* Bold Black */
#define BOLDRED "\033[1m\033[31m"     /* Bold Red */
#define BOLDGREEN "\033[1m\033[32m"   /* Bold Green */
#define BOLDYELLOW "\033[1m\033[33m"  /* Bold Yellow */
#define BOLDBLUE "\033[1m\033[34m"    /* Bold Blue */
#define BOLDMAGENTA "\033[1m\033[35m" /* Bold Magenta */
#define BOLDCYAN "\033[1m\033[36m"    /* Bold Cyan */
#define BOLDWHITE "\033[1m\033[37m"   /* Bold White */
#else
#define RESET "%ESC%[0m"
#define BLACK "%ESC%[30m"       /* Black */
#define RED "%ESC%[31m"         /* Red */
#define GREEN "%ESC%[32m"       /* Green */
#define YELLOW "%ESC%[33m"      /* Yellow */
#define BLUE "%ESC%[34m"        /* Blue */
#define MAGENTA "%ESC%[35m"     /* Magenta */
#define CYAN "%ESC%[36m"        /* Cyan */
#define WHITE "%ESC%[37m"       /* White */
#define BOLDBLACK "%ESC%[90m"   /* Bold Black */
#define BOLDRED "%ESC%[91m"     /* Bold Red */
#define BOLDGREEN "%ESC%[92m"   /* Bold Green */
#define BOLDYELLOW "%ESC%[93m"  /* Bold Yellow */
#define BOLDBLUE "%ESC%[94m"    /* Bold Blue */
#define BOLDMAGENTA "%ESC%[95m" /* Bold Magenta */
#define BOLDCYAN "%ESC%[96m"    /* Bold Cyan */
#define BOLDWHITE "%ESC%[97m"   /* Bold White */
#endif

extern jmp_buf GotoStartScop, GotoEndScop;

namespace macveth {

static inline int MVSkipCode = 11;

/// If condition is false, then print message and goto the jump buffer
static void MVAssertSkip(bool cond, std::string Msg, jmp_buf Goto,
                         int Val = MVSkipCode) {
  if (!cond) {
    std::cerr << BOLDMAGENTA << "[MACVETH SKIPPING] " << RESET << Msg
              << std::endl;
    longjmp(Goto, Val);
  }
}

/// Print message and goto the jump buffer
static void MVSkip(std::string Msg, jmp_buf Goto, int Val = 1) {
  std::cerr << BOLDMAGENTA << "[MACVETH SKIPPING] " << RESET << Msg
            << std::endl;
  longjmp(Goto, Val);
}

/// Print warning message
static void MVWarn(std::string Msg) {
  std::cerr << BOLDYELLOW << "[MACVETH WARNING] " << RESET << Msg << std::endl;
}

/// Print info message
static void MVInfo(std::string Msg) {
  std::cerr << BOLDBLUE << "[MACVETH INFO] " << RESET << Msg << std::endl;
}

/// Print warning if condition not satisfied
static void MVAssertWarn(bool cond, const char *Msg) {
  if (!cond) {
    std::cerr << BOLDRED << "[MACVETH WARNING] " << RESET << Msg << std::endl;
  }
}

/// Hard assertion: if condition not satisfied print error and zeroed-exit
static void MVAssert(bool cond, const char *Msg) {
  if (!cond) {
    std::cerr << BOLDRED << "[MACVETH FATAL ERROR] " << RESET << Msg
              << std::endl;
    exit(0);
  }
}

/// Print error and zeroed-exit
static void MVErr(std::string Msg) {
  std::cerr << BOLDRED << "[MACVETH FATAL ERROR] " << RESET << Msg << std::endl;
  exit(0);
}

} // namespace macveth

#endif /* MACVETH_ASSERT_H */