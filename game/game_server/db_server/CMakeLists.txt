MESSAGE(STATUS "CMake db_server")


CollectSourceFiles(${CMAKE_CURRENT_SOURCE_DIR} PRIVATE_SOURCES)
list(APPEND PRIVATE_SOURCES)
add_executable(db_server ${PRIVATE_SOURCES})


GroupSources(${CMAKE_CURRENT_SOURCE_DIR})
CollectIncludeDirectories(${CMAKE_CURRENT_SOURCE_DIR} PUBLIC_INCLUDES)


target_link_libraries(db_server ${GAME_COMM_LINK_LIB} ${COMM_LINK_LIB})	
#add_dependencies(db_server ${GAME_COMM_LINK_LIB} ${COMM_LINK_LIB})
set_target_properties(db_server PROPERTIES FOLDER "game")

install(TARGETS db_server RUNTIME DESTINATION ${InstallDir})

if(WIN32)
install(FILES  
${SolutionDir}/thirdparty/mongo-c-driver-1.23.3/install/bin/bson-1.0.dll
${SolutionDir}/thirdparty/mongo-c-driver-1.23.3/install/bin/mongoc-1.0.dll
DESTINATION ${InstallDir})
endif()