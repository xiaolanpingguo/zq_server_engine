set(project_name cinatra_press_tool)
project(${project_name})

option(ENABLE_PRESS_TOOL_TESTS "Disable press tool test" OFF)

set(cinatra_press_tool
	main.cpp
	)

add_definitions(-DBENCHMARK_TEST)
add_executable(${project_name} ${cinatra_press_tool})
target_compile_definitions(${project_name} PRIVATE ASYNC_SIMPLE_HAS_NOT_AIO)
target_include_directories(${project_name} PRIVATE
        ${cinatra_SOURCE_DIR}/thirdparty/asio
        ${cinatra_SOURCE_DIR}/thirdparty/async_simple
        ${cinatra_SOURCE_DIR}/thirdparty/cmdline
)

if (ENABLE_SSL)
	target_link_libraries(${project_name} ${OPENSSL_LIBRARIES} pthread -ldl)
endif()

if (ENABLE_CLIENT_SSL)
	target_link_libraries(${project_name} ${OPENSSL_LIBRARIES} pthread -ldl)
endif()

if (ENABLE_GZIP)
	target_link_libraries(${project_name} ${ZLIB_LIBRARIES})
endif()
install(TARGETS ${project_name} DESTINATION include)

set(unittest_press_tool test_cinatra_press_tool)

if (ENABLE_PRESS_TOOL_TESTS)
    add_executable(${unittest_press_tool} main.cpp)

    target_compile_definitions(${unittest_press_tool} PRIVATE ASYNC_SIMPLE_HAS_NOT_AIO PRESS_TOOL_UNITTESTS)
    target_include_directories(${unittest_press_tool} PRIVATE
            ${cinatra_SOURCE_DIR}/thirdparty/asio
            ${cinatra_SOURCE_DIR}/thirdparty/async_simple
            ${cinatra_SOURCE_DIR}/thirdparty/cmdline
            ${cinatra_SOURCE_DIR}/thirdparty/doctest
    )

    if (ENABLE_SSL)
        target_link_libraries(${unittest_press_tool} ${OPENSSL_LIBRARIES} pthread -ldl)
    endif()

    if (ENABLE_CLIENT_SSL)
        target_link_libraries(${unittest_press_tool} ${OPENSSL_LIBRARIES} pthread -ldl)
    endif()

    if (ENABLE_GZIP)
        target_link_libraries(${unittest_press_tool} ${ZLIB_LIBRARIES})
    endif()

endif()