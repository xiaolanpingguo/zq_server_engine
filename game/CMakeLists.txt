message(STATUS "CMake: game")


include_directories(
	${SolutionDir}/thirdparty/protobuf-22.1/install/include
)

link_directories(
	${SolutionDir}/thirdparty/protobuf-22.1/install/lib
	)

if(WIN32)
set(GAME_COMM_LINK_LIB s2s_protocol game_common libprotobufd)
elseif(UNIX)
set(GAME_COMM_LINK_LIB s2s_protocol game_common libprotobuf)
endif()

add_subdirectory(game_common)
add_subdirectory(protocol)
add_subdirectory(game_server)




