#ifndef MACVETH_ASSERT_H
#define MACVETH_ASSERT_H

#include <iostream>
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

namespace macveth {

static void MVWarn(std::string Msg) {
  std::cerr << BOLDYELLOW << "[MACVETH WARNING] " << RESET << Msg << std::endl;
}

static void MVInfo(std::string Msg) {
  std::cerr << BOLDMAGENTA << "[MACVETH INFO] " << RESET << Msg << std::endl;
}

static void MVAssertWarn(bool cond, const char *Msg) {
  if (!cond) {
    std::cerr << BOLDRED << "[MACVETH WARNING] " << RESET << Msg << std::endl;
  }
}

static void MVAssert(bool cond, const char *Msg) {
  if (!cond) {
    std::cerr << BOLDRED << "[MACVETH FATAL ERROR] " << RESET << Msg
              << std::endl;
    exit(0);
  }
}

static void MVErr(std::string Msg) {
  std::cerr << BOLDRED << "[MACVETH FATAL ERROR] " << RESET << Msg << std::endl;
  exit(0);
}

} // namespace macveth

#endif