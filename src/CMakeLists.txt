# set of files to compile and create the library and this stuff
file(GLOB MACVETH_SRC ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
add_library(macveth_lib OBJECT ${MACVETH_SRC})

target_include_directories(macveth_lib PUBLIC ../include ../)
set_property(TARGET macveth_lib PROPERTY ENABLE_EXPORTS 1)

install(TARGETS macveth_lib DESTINATION build)
