set(CMAKE_C_FLAGS " -Wall ")

# Requirements
# find_package(OpenMP REQUIRED)
# # For CMake < 3.9, we need to make the target ourselves
# if(NOT CMAKE_VERSION VERSION_GREATER "3.9")
#     if(NOT TARGET OpenMP::OpenMP_CXX)
#         find_package(Threads REQUIRED)
#         add_library(OpenMP::OpenMP_CXX IMPORTED INTERFACE)
#         set_property(TARGET OpenMP::OpenMP_CXX
#                     PROPERTY INTERFACE_COMPILE_OPTIONS ${OpenMP_CXX_FLAGS})
#         # Only works if the same flag is passed to the linker; use CMake 3.9+ otherwise (Intel, AppleClang)
#         set_property(TARGET OpenMP::OpenMP_CXX
#                     PROPERTY INTERFACE_LINK_LIBRARIES ${OpenMP_CXX_FLAGS} Threads::Threads)
#     endif()
# endif()

# General values
include_directories(../lib)
include_directories(mustpass)

link_libraries(coverage_config)

# # Test singlethread
# add_executable(test_pw_singlethread.o ${PW_LIB} pw_simple.c)

# # No PW_MULTITHREAD but multithread
# add_executable(test_pw_openmp_singlethread.o ${PW_LIB} pw_simple.c)
# target_link_libraries(test_pw_openmp_singlethread.o PRIVATE OpenMP::OpenMP_CXX)
# target_compile_options(test_pw_openmp_singlethread.o PRIVATE "-fopenmp")

# # Test multithread subregions
# add_executable(test_pw_singlethread_subregions.o ${PW_LIB} pw_subregions.c)

# # Tests
# add_test(NAME single COMMAND test_pw_singlethread.o)
# add_test(NAME single_openmp COMMAND test_pw_openmp_singlethread.o)
# add_test(NAME single_subregion COMMAND test_pw_singlethread_subregions.o)