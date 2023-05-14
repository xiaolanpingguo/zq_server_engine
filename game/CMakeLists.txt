message(STATUS "CMake: game")



if(WIN32)

include_directories(
	${SolutionDir}/thirdparty/protobuf-22.1/win-install/include
	${SolutionDir}/thirdparty/mongo-c-driver-1.23.3/win-install/include/libbson-1.0
	${SolutionDir}/thirdparty/mongo-c-driver-1.23.3/win-install/include/libmongoc-1.0
	${SolutionDir}/thirdparty/hiredis-1.1.0/win-install/include
)

link_directories(
	${SolutionDir}/thirdparty/protobuf-22.1/win-install/lib
	${SolutionDir}/thirdparty/mongo-c-driver-1.23.3/win-install/lib
	${SolutionDir}/thirdparty/hiredis-1.1.0/win-install/lib
	)
	
set(GAME_COMM_LINK_LIB s2s_protocol game_common libprotobufd bson-1.0 mongoc-1.0 hiredis_staticd)

install(FILES  
${SolutionDir}/thirdparty/mongo-c-driver-1.23.3/win-install/bin/bson-1.0.dll
${SolutionDir}/thirdparty/mongo-c-driver-1.23.3/win-install/bin/mongoc-1.0.dll
DESTINATION ${InstallDir})

elseif(UNIX)

include_directories(
	${SolutionDir}/thirdparty/protobuf-22.1/linux-install/include
	${SolutionDir}/thirdparty/mongo-c-driver-1.23.3/linux-install/include/libbson-1.0
	${SolutionDir}/thirdparty/mongo-c-driver-1.23.3/linux-install/include/libmongoc-1.0
	${SolutionDir}/thirdparty/hiredis-1.1.0/linux-install/include
)

link_directories(
    ${SolutionDir}/thirdparty/protobuf-22.1/linux-install/lib
	${SolutionDir}/thirdparty/mongo-c-driver-1.23.3/linux-install/lib
	${SolutionDir}/thirdparty/hiredis-1.1.0/linux-install/lib
	)
	
set(GAME_COMM_LINK_LIB s2s_protocol game_common protobuf bson-1.0 mongoc-1.0 hiredis)

# for mongo runtime
install(FILES  
${SolutionDir}/thirdparty/mongo-c-driver-1.23.3/linux-install/lib/libbson-1.0.so
${SolutionDir}/thirdparty/mongo-c-driver-1.23.3/linux-install/lib/libbson-1.0.so.0
${SolutionDir}/thirdparty/mongo-c-driver-1.23.3/linux-install/lib/libbson-1.0.so.0.0.0
${SolutionDir}/thirdparty/mongo-c-driver-1.23.3/linux-install/lib/libmongoc-1.0.so
${SolutionDir}/thirdparty/mongo-c-driver-1.23.3/linux-install/lib/libmongoc-1.0.so.0
${SolutionDir}/thirdparty/mongo-c-driver-1.23.3/linux-install/lib/libmongoc-1.0.so.0.0.0
DESTINATION ${InstallDir})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wl,-rpath=${InstallDir}")

endif()


add_subdirectory(game_common)
add_subdirectory(protocol)
add_subdirectory(game_server)
add_subdirectory(db_mongo)
add_subdirectory(db_redis)
add_subdirectory(test)



