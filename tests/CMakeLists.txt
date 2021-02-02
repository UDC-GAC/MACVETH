#==============================================================================#
#                                                                              #
#                       MACVETH CMake Configuration Test                       #
#                                                                              #
#==============================================================================#

# Include unit test
add_subdirectory(unittest)

# Include integration test
add_subdirectory(integrationtest)

# TODO: Include benchmarks
# add_subdirectory(perftest)