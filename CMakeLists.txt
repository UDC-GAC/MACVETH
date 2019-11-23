# Minimum version required
cmake_minimum_required(VERSION 3.13)

# Project name
set(PROJECT_NAME MACVETH)
project(${PROJECT_NAME} VERSION 0.1)

set(CMAKE_CXX_STANDARD 17)

# Setting some paths, for comodity
set(LLVM_BUILD_PATH /usr/lib/llvm-9)
set(LLVM_BIN_PATH ${LLVM_BUILD_PATH}/bin)
set(CURSES_USE_NCURSES TRUE)

#add_library(macveth_flags INTERFACE)
#target_link_libraries(macveth_flags INTERFACE
#    "$<BUILD_INTERFACE:-L/usr/lib/llvm-9/lib>")

############################################################
# Setting CXXFLAGS using llvm-config + custom flags
# CXXFLAGS + LLVM_CXXFLAGS (former makefile)
execute_process(
    COMMAND ${LLVM_BIN_PATH}/llvm-config --cxxflags
    OUTPUT_VARIABLE LLVM_CXXFLAGS)
set(LLVM_CXXFLAGS ${LLVM_CXXFLAGS} " -std=c++17 -fno-rtti -O0 -L/usr/lib/llvm-9/lib")
add_definitions(${LLVM_CXXFLAGS})

############################################################
# Libraries Dynamic
execute_process(
    COMMAND ${LLVM_BIN_PATH}/llvm-config --ldflags --libs --system-libs
    OUTPUT_VARIABLE CLANG_LIBS)
add_definitions(${CLANG_LIBS})

############################################################
# include dirs
execute_process(
    COMMAND ${LLVM_BIN_PATH}/llvm-config --includedir
    OUTPUT_VARIABLE CLANG_INCLUDE_DIR)
find_library(LibClang_LIBRARY NAMES clang HINTS /usr/lib/llvm-9/lib)
set(LibClang_LIBRARIES ${LibClang_LIBRARY})
set(LibClang_INCLUDE_DIRS ${CLANG_INCLUDE_DIR})

# configuration file, for macros for instance
configure_file(macveth_translator.h.in src/macveth_translator.h)

# add a subdirectory to the buildd
add_subdirectory(src)

# define the binary to compile
add_executable(MACVETH_MAIN
    lib/macveth_translator.cpp
    )

# CLANG_INCLUDES (former Makefile)
target_include_directories(MACVETH_MAIN PUBLIC
    # ${CLANG_INCLUDE_DIR}
    include
    /usr/lib/llvm-9/build/include/clang
    /usr/lib/llvm-9/build/clang/include)

# Custom libraries needed
target_link_libraries(MACVETH_MAIN
    PRIVATE
    macveth_lib
    #    macveth_flags
    ${LibClang_LIBRARIES}
    -L/usr/lib/llvm-9/lib)

# Clang libraries needed
target_link_libraries(MACVETH_MAIN
    PRIVATE
    clangTooling
    clangBasic
    clangASTMatchers
    clangAnalysis
    clangAST
    clangASTMatchers
    clangAnalysis
    clangBasic
    clangDriver
    clangEdit
    clangFrontend
    clangFrontendTool
    clangLex
    clangParse
    clangSema
    clangEdit
    clangRewrite
    clangRewriteFrontend
    clangStaticAnalyzerFrontend
    clangStaticAnalyzerCheckers
    clangStaticAnalyzerCore
    clangCrossTU
    clangIndex
    clangSerialization
    clangToolingCore
    clangTooling
    clangFormat
    )

# LLVM flags needed
target_link_libraries(MACVETH_MAIN
    PRIVATE
    LLVM)
##target_link_libraries(MACVETH_MAIN
##    PUBLIC
##    LLVMX86AsmParser # MC, MCParser, Support, X86Desc, X86Info
##    LLVMX86Desc # MC, Support, X86AsmPrinter, X86Info
##    LLVMX86Info # MC, Support, Target
##    LLVMX86Utils # Core, Support
##    LLVMipo
##    LLVMScalarOpts
##    LLVMInstCombine
##    LLVMTransformUtils
##    LLVMAnalysis
##    LLVMTarget
##    LLVMOption # Support
##    LLVMMCParser # MC, Support
##    LLVMMC # Object, Support
##    LLVMObject # BitReader, Core, Support
##    LLVMBitReader # Core, Support
##    LLVMCore # Support
##    LLVMSupport
##    LLVM
##    )

# needed flags
target_link_libraries(MACVETH_MAIN
    PUBLIC
    pthread
    z
    tinfo
    dl
    )

# tell to compiler MACVETH where to place the binary
install(TARGETS MACVETH_MAIN DESTINATION build)

