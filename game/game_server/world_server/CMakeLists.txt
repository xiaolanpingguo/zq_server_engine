MESSAGE(STATUS "CMake world_server")


CollectSourceFiles(${CMAKE_CURRENT_SOURCE_DIR} PRIVATE_SOURCES)
list(APPEND PRIVATE_SOURCES)
add_executable(world_server ${PRIVATE_SOURCES})


GroupSources(${CMAKE_CURRENT_SOURCE_DIR})
CollectIncludeDirectories(${CMAKE_CURRENT_SOURCE_DIR} PUBLIC_INCLUDES)


target_link_libraries(world_server c2s_protocol ${GAME_COMM_LINK_LIB} ${COMM_LINK_LIB})	
#add_dependencies(world_server ${GAME_COMM_LINK_LIB} ${COMM_LINK_LIB})
set_target_properties(world_server PROPERTIES FOLDER "game")

install(TARGETS world_server RUNTIME DESTINATION ${InstallDir})