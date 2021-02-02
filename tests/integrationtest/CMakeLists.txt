#==============================================================================#
#                                                                              #
#                       MACVETH CMake Integration Test                         #
#                                                                              #
#==============================================================================#

# For simplicity
macro(add_integration_test_macveth path_test name_test)
    # Now simply link against gtest or gtest_main as needed. Eg
    add_test(NAME integration_${name_test} 
        COMMAND python3 macveth_testing.py ${path_test} ${name_test} 
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
endmacro()


# Fulltest
file(GLOB TEST_LIST fulltest/test_*.c)
foreach(TEST_PATH IN LISTS TEST_LIST)
    get_filename_component(TEST_NAME ${TEST_PATH} NAME)
    add_integration_test_macveth(fulltest/ ${TEST_NAME})
endforeach()

# Mustpass
file(GLOB TEST_LIST mustpass/must_*.c)
foreach(TEST_PATH IN LISTS TEST_LIST)
    get_filename_component(TEST_NAME ${TEST_PATH} NAME)
    add_integration_test_macveth(mustpass/ ${TEST_NAME})
endforeach()