MESSAGE(STATUS "CMake test")


CollectSourceFiles(${CMAKE_CURRENT_SOURCE_DIR} PRIVATE_SOURCES)
list(APPEND PRIVATE_SOURCES)
add_executable(test ${PRIVATE_SOURCES})


GroupSources(${CMAKE_CURRENT_SOURCE_DIR})
CollectIncludeDirectories(${CMAKE_CURRENT_SOURCE_DIR} PUBLIC_INCLUDES)


target_link_libraries(test ${GAME_COMM_LINK_LIB} ${COMM_LINK_LIB})	
set_target_properties(test PROPERTIES FOLDER "game")

install(TARGETS test RUNTIME DESTINATION ${InstallDir})