message(STATUS "CMake: game")


include_directories(
	${SolutionDir}/thirdparty/protobuf-22.1/install/include
	${SolutionDir}/thirdparty/mongo-c-driver-1.23.3/install/include/libbson-1.0
	${SolutionDir}/thirdparty/mongo-c-driver-1.23.3/install/include/libmongoc-1.0
	${SolutionDir}/thirdparty/hiredis-1.1.0/install/include
)

link_directories(
	${SolutionDir}/thirdparty/protobuf-22.1/install/lib
	${SolutionDir}/thirdparty/mongo-c-driver-1.23.3/install/lib
	${SolutionDir}/thirdparty/hiredis-1.1.0/install/lib
	)

if(WIN32)
set(GAME_COMM_LINK_LIB s2s_protocol game_common libprotobufd bson-1.0 mongoc-1.0 hiredis_staticd)
elseif(UNIX)
set(GAME_COMM_LINK_LIB s2s_protocol game_common libprotobuf bson-1.0 mongoc-1.0 hiredis)
endif()

if(WIN32)
install(FILES  
${SolutionDir}/thirdparty/mongo-c-driver-1.23.3/install/bin/bson-1.0.dll
${SolutionDir}/thirdparty/mongo-c-driver-1.23.3/install/bin/mongoc-1.0.dll
DESTINATION ${InstallDir})
endif()

add_subdirectory(game_common)
add_subdirectory(protocol)
add_subdirectory(game_server)
add_subdirectory(db_mongo)
add_subdirectory(db_redis)
add_subdirectory(test)



