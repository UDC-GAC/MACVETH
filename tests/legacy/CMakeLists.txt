# Minimum version required
cmake_minimum_required(VERSION 3.13 FATAL_ERROR)

# Minimum C++ std
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Project name
set(PROJECT_NAME bench)

# Poly
set(POLY_FLAGS " -DPOLYBENCH_TIME -O2")
add_definitions(${POLY_FLAGS})

# Standard
file(GLOB CONV_SRC conv/*)
add_library(conv_lib OBJECT ${CONV_SRC})

# AVX
file(GLOB CONVAVX_SRC conv-avx/*)
add_library(convavx_lib OBJECT ${CONVAVX_SRC})
target_compile_options(
    convavx_lib PRIVATE -mavx -mfma)

# bench
add_executable(bench
    main.c
    utilities/polybench.c
    )

target_link_libraries(bench PRIVATE
    conv_lib
    )

target_include_directories(bench PUBLIC
    utilities
    .
    )

# bench AVX
add_executable(bench_avx
    main.c
    utilities/polybench.c
    )

target_compile_options(
    bench_avx PRIVATE -DCONV_AVX)

target_link_libraries(bench_avx PRIVATE
    convavx_lib
    )

target_include_directories(bench PUBLIC
    utilities
    .
    )

# tell to compiler bench where to place the binary
install(TARGETS bench DESTINATION build)
install(TARGETS bench_avx DESTINATION build)

