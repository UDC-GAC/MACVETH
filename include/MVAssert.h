// MACVETH - MVAssert.h
//
// Copyright (c) Colorado State University. 2019-2021
// Copyright (c) Universidade da Coruña. 2020-2021
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// Authors:
//     Marcos Horro <marcos.horro@udc.es>
//     Louis-Nöel Pouchet <pouchet@colostate.edu>
//     Gabriel Rodríguez <grodriguez@udc.es>
//
// Contact:
//     Louis-Nöel Pouchet <pouchet@colostate.edu>

#ifndef MACVETH_ASSERT_H
#define MACVETH_ASSERT_H

#include <iostream>
#include <setjmp.h>
#include <string>

#ifndef WIN32
// the following are UBUNTU/LINUX, and MacOS ONLY terminal color codes.
#define RESET "\033[0m"
//#define RED "\033[31m"                /* Red */
//#define GREEN "\033[32m"              /* Green */
//#define YELLOW "\033[33m"             /* Yellow */
//#define BLUE "\033[34m"               /* Blue */
//#define MAGENTA "\033[35m"            /* Magenta */
#define BOLDRED "\033[1m\033[31m"     /* Bold Red */
#define BOLDGREEN "\033[1m\033[32m"   /* Bold Green */
#define BOLDYELLOW "\033[1m\033[33m"  /* Bold Yellow */
#define BOLDBLUE "\033[1m\033[34m"    /* Bold Blue */
#define BOLDMAGENTA "\033[1m\033[35m" /* Bold Magenta */
#else
#define RESET "%ESC%[0m"
#define RED "%ESC%[31m"         /* Red */
#define GREEN "%ESC%[32m"       /* Green */
#define YELLOW "%ESC%[33m"      /* Yellow */
#define BLUE "%ESC%[34m"        /* Blue */
#define MAGENTA "%ESC%[35m"     /* Magenta */
#define BOLDRED "%ESC%[91m"     /* Bold Red */
#define BOLDGREEN "%ESC%[92m"   /* Bold Green */
#define BOLDYELLOW "%ESC%[93m"  /* Bold Yellow */
#define BOLDBLUE "%ESC%[94m"    /* Bold Blue */
#define BOLDMAGENTA "%ESC%[95m" /* Bold Magenta */
#endif

extern jmp_buf GotoStartScop, GotoEndScop;

namespace macveth {

static inline int MVSkipCode = 11;

/// If condition is false, then print message and goto the jump buffer
void MVAssertSkip(bool cond, std::string Msg, jmp_buf Goto,
                  int Val = MVSkipCode);
/// Print warning message
void MVWarn(std::string Msg);
/// Print info message
void MVInfo(std::string Msg);
/// Hard assertion: if condition not satisfied print error and zeroed-exit
void MVAssert(bool cond, const char *Msg);
/// Print error and zeroed-exit
void MVErr(std::string Msg);

} // namespace macveth

#endif /* MACVETH_ASSERT_H */