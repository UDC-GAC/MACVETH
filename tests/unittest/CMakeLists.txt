#==============================================================================#
#                                                                              #
#                       MACVETH CMake Unit Test Config                         #
#                                                                              #
#==============================================================================#


set(AddGTest_DIR ${CMAKE_CURRENT_SOURCE_DIR})
find_package(AddGTest REQUIRED)

# For simplicity
macro(add_test_macveth name_test)
    # Now simply link against gtest or gtest_main as needed. Eg
    add_executable(${name_test} ${name_test}.cpp)   
    target_link_libraries(${name_test} gtest_main)
    add_test(NAME ${name_test}_gtest COMMAND ${name_test})
endmacro()

# Adding all unit tests
add_test_macveth(TAC_test)