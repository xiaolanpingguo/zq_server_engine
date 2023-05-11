MESSAGE(STATUS "CMake zone_server")


CollectSourceFiles(${CMAKE_CURRENT_SOURCE_DIR} PRIVATE_SOURCES)
list(APPEND PRIVATE_SOURCES)
add_executable(zone_server ${PRIVATE_SOURCES})


GroupSources(${CMAKE_CURRENT_SOURCE_DIR})
CollectIncludeDirectories(${CMAKE_CURRENT_SOURCE_DIR} PUBLIC_INCLUDES)


target_link_libraries(zone_server c2s_protocol db_mongo ${GAME_COMM_LINK_LIB} ${COMM_LINK_LIB})	
set_target_properties(zone_server PROPERTIES FOLDER "game")

install(TARGETS zone_server RUNTIME DESTINATION ${InstallDir})