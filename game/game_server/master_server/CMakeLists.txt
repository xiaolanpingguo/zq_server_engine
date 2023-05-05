MESSAGE(STATUS "CMake master_server")


CollectSourceFiles(${CMAKE_CURRENT_SOURCE_DIR} PRIVATE_SOURCES)
list(APPEND PRIVATE_SOURCES)
add_executable(master_server ${PRIVATE_SOURCES})


GroupSources(${CMAKE_CURRENT_SOURCE_DIR})
CollectIncludeDirectories(${CMAKE_CURRENT_SOURCE_DIR} PUBLIC_INCLUDES)


target_link_libraries(master_server ${COMM_LINK_LIB} ${GAME_COMM_LINK_LIB})	
#add_dependencies(master_server ${GAME_COMM_LINK_LIB} ${COMM_LINK_LIB})
set_target_properties(master_server PROPERTIES FOLDER "game")

install(TARGETS master_server RUNTIME DESTINATION ${InstallDir})