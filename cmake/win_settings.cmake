add_definitions(-D_WIN32_WINNT=0x0601)
add_definitions(-DWIN32_LEAN_AND_MEAN)
add_definitions(-DNOMINMAX)

if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  include(${CMAKE_SOURCE_DIR}/cmake/msvc_settings.cmake)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  include(${CMAKE_SOURCE_DIR}/cmake/clang_settings.cmake)
endif()
