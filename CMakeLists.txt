# Minimum version required
cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

# Minimum C++ std
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

# Project name
set(PROJECT_NAME MACVETH)
project(${PROJECT_NAME} VERSION 0.1.0)


############################################################
# TODO: LLVM version
# execute_process(
#     COMMAND llvm-config --version
#     OUTPUT_VARIABLE LLVM_VER)
# message("-- Using LLVM ${LLVM_VER}")


set(LLVM_VER llvm-10)

# Setting some paths, for comodity
set(LLVM_BUILD_PATH /usr/lib/${LLVM_VER})
set(LLVM_BIN_PATH ${LLVM_BUILD_PATH}/bin)
set(CURSES_USE_NCURSES TRUE)

# Custom flags
set(CUSTOM_CXXFLAGS " -g -fno-rtti -Ofast ")
add_definitions(${CUSTOM_CXXFLAGS})

############################################################
# Check if Doxygen is installed
find_package(Doxygen)
if (DOXYGEN_FOUND)
    # set input and output files
    set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/doc/Doxyfile.in)
    set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)
    set(abs_srcdir ${CMAKE_CURRENT_SOURCE_DIR})
    set(abs_builddir ${CMAKE_CURRENT_BINARY_DIR})

    # request to configure the file
    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
    message("Doxygen build started")

    # note the option ALL which allows to build the docs together with the application
    add_custom_target(macveth_doxygen ALL
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM)
else (DOXYGEN_FOUND)
    message("Doxygen need to be installed to generate the doxygen documentation")
endif (DOXYGEN_FOUND)

############################################################
# Coverage report
add_library(coverage_config INTERFACE)

option(CODE_COVERAGE "Enable coverage reporting" OFF)
if(CODE_COVERAGE AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    # Add required flags (GCC & LLVM/Clang)
    target_compile_options(coverage_config INTERFACE
        -O0        # no optimization
        -g         # generate debug info
        --coverage # sets all required flags
    )
    if(CMAKE_VERSION VERSION_GREATER "3.16")
        target_link_options(coverage_config INTERFACE --coverage)
    else()
        target_link_libraries(coverage_config INTERFACE --coverage)
    endif()
endif()

############################################################
# Testing
option (BUILD_TESTING "Build the testing tree." ON)
# Only build tests if we are the top-level project
# Allows this to be used by super projects with `add_subdirectory`
if (BUILD_TESTING AND (PROJECT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR))
    enable_testing()
    add_subdirectory(tests)
endif()


############################################################
# Setting CXXFLAGS using llvm-config + custom flags
# CXXFLAGS + LLVM_CXXFLAGS (former makefile)
execute_process(
    COMMAND ${LLVM_BIN_PATH}/llvm-config --cxxflags
    OUTPUT_VARIABLE LLVM_CXXFLAGS)
set(LLVM_CXXFLAGS ${LLVM_CXXFLAGS})
add_definitions(${LLVM_CXXFLAGS})

############################################################
# Libraries Dynamic
# execute_process(
#     COMMAND ${LLVM_BIN_PATH}/llvm-config --ldflags --libs --system-libs
#     OUTPUT_VARIABLE CLANG_LIBS)
# add_definitions(${CLANG_LIBS})

############################################################
# include dirs
execute_process(
    COMMAND ${LLVM_BIN_PATH}/llvm-config --includedir
    OUTPUT_VARIABLE CLANG_INCLUDE_DIR)
find_library(LibClang_LIBRARY NAMES clang HINTS /usr/lib/${LLVM_VER}/lib)
set(LibClang_LIBRARIES ${LibClang_LIBRARY})
set(LibClang_INCLUDE_DIRS ${CLANG_INCLUDE_DIR})

if (NOT DEFINED LLVM_DIR)
    set(LLVM_DIR /usr/lib/llvm)
endif()

include_directories(
     ${LLVM_DIR}/build/llvm/
     ${LLVM_DIR}/clang/include
     ${LLVM_DIR}/build/tools/clang/include
)

# for generating compile_options.json for IDEs
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

# configuration file, for macros for instance
set(LLVM_VERSION 10)
configure_file(macveth.h.in lib/macveth.h)

# add a subdirectory to the buildd
add_subdirectory(src)

# define the binary to compile
add_executable(macveth
    lib/macveth.cpp
    )

# CLANG_INCLUDES (former Makefile)
target_include_directories(macveth PUBLIC
    include
    /usr/lib/${LLVM_VER}/build/include/clang
    )

# Custom libraries needed
target_link_libraries(macveth
    PRIVATE
    macveth_lib
    ${LibClang_LIBRARIES}
    -L/usr/lib/${LLVM_VER}/lib)

# Clang libraries needed
target_link_libraries(macveth PRIVATE
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
target_link_libraries(macveth
    PRIVATE
    LLVM
    )

# needed flags
target_link_libraries(macveth
    PUBLIC
    pthread
    z
    tinfo
    dl
    stdc++fs # needed in some gcc versions
    )

# tell to compiler MACVETH where to place the binary
install(TARGETS macveth DESTINATION build)

