#==============================================================================#
#                                                                              #
#                       MACVETH CMake Integration Test                         #
#                                                                              #
#==============================================================================#

# Defining macro for simplicity:
macro(add_integration_test_macveth path_test name_test)
    add_test(NAME integration_${name_test} 
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
    add_integration_test_macveth(mustpass/ ${TEST_NAME})
endforeach()

# "micro_codelets": these are very specific to ensure correctness in all
# codelet shapes
file(GLOB TEST_LIST micro_codelets/patterns/codelet_*.c)
foreach(TEST_PATH IN LISTS TEST_LIST)
    get_filename_component(TEST_NAME ${TEST_PATH} NAME)
    add_integration_test_macveth(micro_codelets/ micro_${TEST_NAME})
endforeach()