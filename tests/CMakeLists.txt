set(CMAKE_C_FLAGS " -Wall ")

# General values
include_directories(../lib)
include_directories(mustpass)
include_directories(macveth_dir)

link_libraries(coverage_config)

# # Test multithread subregions
# add_executable(test_pw_singlethread_subregions.o ${PW_LIB} pw_subregions.c)

# # Tests
# add_test(NAME single COMMAND test_pw_singlethread.o)
# add_test(NAME single_openmp COMMAND test_pw_openmp_singlethread.o)
# add_test(NAME single_subregion COMMAND test_pw_singlethread_subregions.o)