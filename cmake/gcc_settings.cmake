set(GCC_EXPECTED_VERSION 11.1.0)

if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS GCC_EXPECTED_VERSION)
  message(FATAL_ERROR "GCC: requires version ${GCC_EXPECTED_VERSION} to build but found ${CMAKE_CXX_COMPILER_VERSION}")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ggdb3 -O0 -m64 -Wall -Wno-reorder -ldl -fPIC -std=c++20")
set(CMAKE_C_FLAGS " -ggdb3 -O0 -m64 -Wall -ldl -fPIC")




