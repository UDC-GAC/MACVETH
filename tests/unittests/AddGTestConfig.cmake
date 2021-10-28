#==============================================================================#
#                                                                              #
#                       MACVETH CMake Unit Test Config                         #
#                                                                              #
#==============================================================================#

if (${LLVM_BUILD_TYPE} MATCHES Debug)
  message(STATUS "No need to compile again googletest, as LLVM built type is Debug")
else()
  # Fetching content
  message(STATUS "Fetching googletest")
  include(FetchContent)
  # Need to change the hash of the file
  FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
  )
  # For Windows: Prevent overriding the parent project's compiler/linker settings
  set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
  message(STATUS "Setting up googletest")
  FetchContent_MakeAvailable(googletest)
endif()
