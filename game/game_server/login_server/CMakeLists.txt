MESSAGE(STATUS "CMake login_server")


CollectSourceFiles(${CMAKE_CURRENT_SOURCE_DIR} PRIVATE_SOURCES)
list(APPEND PRIVATE_SOURCES)
add_executable(login_server ${PRIVATE_SOURCES})


GroupSources(${CMAKE_CURRENT_SOURCE_DIR})
CollectIncludeDirectories(${CMAKE_CURRENT_SOURCE_DIR} PUBLIC_INCLUDES)


target_link_libraries(login_server c2s_protocol ${GAME_COMM_LINK_LIB} ${COMM_LINK_LIB})	
set_target_properties(login_server PROPERTIES FOLDER "game")

install(TARGETS login_server RUNTIME DESTINATION ${InstallDir})
