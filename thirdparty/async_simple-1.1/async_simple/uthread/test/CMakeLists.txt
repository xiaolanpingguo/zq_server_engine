file(GLOB uthread_test_src "*.cpp")

if (CMAKE_BUILD_TYPE STREQUAL "Debug" AND CMAKE_CXX_COMPILER_ID STREQUAL "Clang" AND
    CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL "13")
endif()

add_executable(async_simple_uthread_test ${uthread_test_src} ${PROJECT_SOURCE_DIR}/async_simple/test/dotest.cpp)

target_link_libraries(async_simple_uthread_test async_simple ${deplibs} ${testdeplibs})

add_test(NAME run_async_simple_uthread_test COMMAND async_simple_uthread_test)

