# set of files to compile and create the library and this stuff
file(GLOB MACVETH_SRC ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
add_library(macveth_lib OBJECT ${MACVETH_SRC})

############################################################
# Setting CXXFLAGS using llvm-config + custom flags
# CXXFLAGS + LLVM_CXXFLAGS (former makefile)
execute_process(
    COMMAND ${LLVM_BIN_PATH}/llvm-config --cxxflags
    OUTPUT_VARIABLE LLVM_CXXFLAGS)
set(LLVM_CXXFLAGS ${LLVM_CXXFLAGS} " -std=c++17 -fno-rtti -O0 -L/usr/lib/llvm-9/lib")
add_definitions(${LLVM_CXXFLAGS})

target_include_directories(macveth_lib PUBLIC ../include ../ /usr/lib/llvm-9/include)
set_property(TARGET macveth_lib PROPERTY ENABLE_EXPORTS 1)

install(TARGETS macveth_lib DESTINATION build)
