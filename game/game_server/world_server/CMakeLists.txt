MESSAGE(STATUS "CMake world_server")


CollectSourceFiles(${CMAKE_CURRENT_SOURCE_DIR} PRIVATE_SOURCES)
list(APPEND PRIVATE_SOURCES)
add_executable(world_server ${PRIVATE_SOURCES})


GroupSources(${CMAKE_CURRENT_SOURCE_DIR})
CollectIncludeDirectories(${CMAKE_CURRENT_SOURCE_DIR} PUBLIC_INCLUDES)


target_link_libraries(world_server ${COMM_LINK_LIB})	
add_dependencies(${COMM_LINK_LIB})
set_target_properties(world_server PROPERTIES FOLDER "game")