#==============================================================================#
#                                                                              #
#                       MACVETH CMake Integration Test                         #
#                                                                              #
#==============================================================================#

# Defining macro for simplicity:
macro(add_integration_test_macveth path_test name_test prefix_test)
    add_test(NAME integration_${prefix_test}_${name_test} 
        COMMAND python3 macveth_testing.py ${path_test} ${name_test} 
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
endmacro()


# TODO: "fulltest"
# file(GLOB TEST_LIST fulltest/test_*.c)
# foreach(TEST_PATH IN LISTS TEST_LIST)
#     get_filename_component(TEST_NAME ${TEST_PATH} NAME)
#     add_integration_test_macveth(fulltest/ ${TEST_NAME})
# endforeach()

# "mustpass" tests: those which can not ever fail
file(GLOB TEST_LIST mustpass/must_*.c)
foreach(TEST_PATH IN LISTS TEST_LIST)
    get_filename_component(TEST_NAME ${TEST_PATH} NAME)
    add_integration_test_macveth(mustpass/ ${TEST_NAME} mustpass)
endforeach()

file(GLOB TEST_LIST randompacking/randompacking_*.c)
foreach(TEST_PATH IN LISTS TEST_LIST)
    get_filename_component(TEST_NAME ${TEST_PATH} NAME)
    add_integration_test_macveth(randompacking/ ${TEST_NAME} randompacking)
endforeach()


# "micro_codelets": these are very specific to ensure correctness in all
# codelet shapes
# file(GLOB TEST_LIST micro_codelets_double/patterns/codelet_*.c)
# foreach(TEST_PATH IN LISTS TEST_LIST)
#     get_filename_component(TEST_NAME ${TEST_PATH} NAME)
#     add_integration_test_macveth(micro_codelets_double/ micro_${TEST_NAME} double)
# endforeach()

# file(GLOB TEST_LIST micro_codelets_float/patterns/codelet_*.c)
# foreach(TEST_PATH IN LISTS TEST_LIST)
#     get_filename_component(TEST_NAME ${TEST_PATH} NAME)
#     add_integration_test_macveth(micro_codelets_float/ micro_${TEST_NAME} float)
# endforeach()