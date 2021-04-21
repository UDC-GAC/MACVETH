#===============================================================================
#                                                                              #
#                         MACVETH CMake Configuration                          #
#                                                                              #
#===============================================================================

# Minimum CMake version required
cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

#===============================================================================
# PROJECT VARIABLES
#===============================================================================
# Minimum C++ std
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

# Project name
set(PROJECT_NAME MACVETH)

# Project version
set(MACVETH_VERSION_MAJOR 0)
set(MACVETH_VERSION_MINOR 1)
set(MACVETH_VERSION_PATCH 0)
set(MACVETH_VERSION
    ${MACVETH_VERSION_MAJOR}.${MACVETH_VERSION_MINOR}.${MACVETH_VERSION_PATCH})

# Setup project
project(${PROJECT_NAME} VERSION ${MACVETH_VERSION})
message(STATUS
    "Generating building files for ${PROJECT_NAME} v${MACVETH_VERSION}")

#===============================================================================
# BUILD TYPE, Compiler used and project flags
# -DCMAKE_BUILD_TYPE="Debug|Release|RelWithDebInfo|MinSizeRel"
#===============================================================================

# Set a default build type if none was specified
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(WARNING "Setting build type to 'Release' as none was specified.")
  set(CMAKE_BUILD_TYPE "Release" CACHE
      STRING "Choose the type of build." FORCE)
  # Set the possible values of build type for cmake-gui
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
    "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
else()
    message("Setting build type to " ${CMAKE_BUILD_TYPE})
endif()

if (CMAKE_BUILD_TYPE MATCHES Debug)
    # Compiler flags
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall\
        -fdiagnostics-color=always")
endif()

#===============================================================================
# DOCUMENTATION GENERATION
# -DGENERATE_DOC:BOOL=TRUE to activate
#===============================================================================
# Check if Doxygen is installed
option(GENERATE_DOC "Generate documentation with Doxygen" OFF)

# dot requirements:
# https://github.com/labapart/gattlib/issues/129#issuecomment-609394695
find_package(Doxygen REQUIRED dot)

if ((CMAKE_BUILD_TYPE MATCHES Debug) AND GENERATE_DOC)
    # set input and output files
    set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/doc/Doxyfile.cfg)
    set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)
    set(abs_srcdir ${CMAKE_CURRENT_SOURCE_DIR})
    set(abs_builddir ${CMAKE_CURRENT_BINARY_DIR})

    # request to configure the file
    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
    message(STATUS "Documentation: Doxygen build started")

    # note the option ALL which allows to build the docs together with 
    # the application
    add_custom_target(macveth_doxygen ALL
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM)
elseif(NOT DOXYGEN_FOUND)
    message(WARNING
    "Documentation: Doxygen need to be installed to generate the doxygen documentation")
endif()

#===============================================================================
# CODE COVERAGE CONFIGURATION
# -DCODE_COVERAGE:BOOL=TRUE to activate
#===============================================================================
option(CODE_COVERAGE "Enable coverage reporting" OFF)
add_library(coverage_config INTERFACE)
if((CMAKE_BUILD_TYPE MATCHES Debug) OR
    (CODE_COVERAGE AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang"))
    message(STATUS "Code coverage: enabled")
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

#===============================================================================
# TESTING CONFIGURATION
# -DBUILD_TESTING:BOOL=TRUE to activate
#===============================================================================
option(BUILD_TESTING "Build the testing tree" OFF)
# Only build tests if we are the top-level project
# Allows this to be used by super projects with `add_subdirectory`
if ((CMAKE_BUILD_TYPE MATCHES Debug) OR
    (BUILD_TESTING AND (PROJECT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)))
    message(STATUS "Testing enabled")
    enable_testing()
    add_subdirectory(tests)
endif()


#===============================================================================
# LOAD CLANG/LLVM CONFIGURATION
# For more info: http://llvm.org/docs/CMake.html#embedding-llvm-in-your-project
#===============================================================================

# IMPORTANT NOTE:
# If you have installed LLVM locally using CMake, then you need to pass
# variable -DLLVM_DIR=<path> to the cmake building command, where <path> is the
# path returned by your llvm-config --cmakedir

# This list must be in descendant order. This is just a hack...
set(TESTED_LLVM_VERSIONS 12 11 10)
foreach(LLVM_COMPATIBLE_VERSION ${TESTED_LLVM_VERSIONS})
    # CONFIG file (LLVMConfig.cmake), do not fail [QUIET] if not found: maybe
    # we do not have 11.x but we have 10.x
    find_package(LLVM ${LLVM_COMPATIBLE_VERSION} CONFIG QUIET)
    if (DEFINED LLVM_FOUND AND LLVM_FOUND EQUAL 1)
        break()
    endif()
endforeach()

# LLVM not found in any version.
if (DEFINED LLVM_FOUND AND LLVM_FOUND EQUAL 0)
    message(FATAL_ERROR "Please, install LLVM/Clang >= 10.x")
endif()

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

# This is a hack, actually...
set(Clang_DIR ${LLVM_LIBRARY_DIR}/cmake/clang)
find_package(Clang REQUIRED HINTS ${Clang_DIR} NO_DEFAULT_PATH)
if (NOT DEFINED Clang_FOUND)
    message(FATAL_ERROR "Clang library not found. Check your paths.")
endif()
message(STATUS "Using ClangConfig.cmake in: ${CLANG_CMAKE_DIR}")

# Setting LLVM/Clang paths to CMake's
set(CMAKE_MODULE_PATH
  ${CMAKE_MODULE_PATH}
  "${LLVM_CMAKE_DIR}"
  "${CLANG_CMAKE_DIR}"
  )

# Import LLVM CMake functions
include(AddLLVM)

list(APPEND CMAKE_PREFIX_PATH "${LLVM_CMAKE_DIR}")
list(APPEND CMAKE_PREFIX_PATH "${CLANG_CMAKE_DIR}")

# Set the LLVM and Clang header and library paths
include_directories(SYSTEM "${LLVM_INCLUDE_DIRS};${CLANG_INCLUDE_DIRS}")

# LLVM/Clang is normally built without RTTI. Be consistent with that.
if(NOT LLVM_ENABLE_RTTI)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti")
endif()

# For generating compile_options.json for IDEs
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

# Configuration file, for macros for instance
configure_file(macveth.h.in lib/macveth.h)

# Add a subdirectory to the build
add_subdirectory(src)

# Define the binary to compile
add_executable(macveth
    lib/macveth.cpp
    )

#===============================================================================
# BUILDING TARGET
#===============================================================================

find_package(Threads REQUIRED)  # Threads library

# Configuration file
configure_file(macveth.h.in lib/macveth.h)

# Include directories
target_include_directories(macveth PUBLIC
    include
    ${CLANG_INCLUDE_DIRS}
    ${PROJECT_BINARY_DIR}
    )

# Custom libraries needed
target_link_libraries(macveth
    PRIVATE
    macveth_lib
    coverage_config # need for code coverage. Only built if -DCODE_COVERAGE
    )

# MACVETH Clang libraries dependencies
# Clang libraries needed
if(CLANG_LINK_CLANG_DYLIB)
    message(STATUS "Linking with clang-cpp library")
    target_link_libraries(macveth PRIVATE clang-cpp)
else()
    target_link_libraries(macveth PRIVATE
        clangTooling
        clangBasic
        clangASTMatchers
        clangAnalysis
        clangAST
        clangASTMatchers
        clangBasic
        clangDriver
        clangEdit
        clangFrontend
        clangFrontendTool
        clangLex
        clangParse
        clangSema
        clangRewrite
        clangRewriteFrontend
        clangStaticAnalyzerFrontend
        clangStaticAnalyzerCheckers
        clangStaticAnalyzerCore
        clangCrossTU
        clangIndex
        clangSerialization
        clangToolingCore
        clangToolingInclusions
        clangTooling
        clangFormat
        )
endif()

# LLVM flags needed
target_link_libraries(macveth
    PRIVATE
    LLVM
    )

# needed flags
target_link_libraries(macveth
    PUBLIC
    pthread
    tinfo
    dl
    stdc++fs # needed in some gcc versions
    )

#===============================================================================
# INSTALLING TARGET
#===============================================================================
install(TARGETS macveth DESTINATION build)

