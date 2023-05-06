cmake_minimum_required(VERSION 3.1)
project(zq_server_engine)


message(STATUS "CMAKE_SYSTEM_NAME          = ${CMAKE_SYSTEM_NAME}"         )
message(STATUS "CMAKE_CXX_COMPILER_ID      = ${CMAKE_CXX_COMPILER_ID}"     )
message(STATUS "CMAKE_CXX_COMPILER_VERSION = ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "CMAKE_COMPILER_IS_GNUCXX   = ${CMAKE_COMPILER_IS_GNUCXX}"  )


set(CMAKE_DISABLE_SOURCE_CHANGES ON)
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)
set(CMAKE_CXX_STANDARD 20)


set(SolutionDir ${PROJECT_SOURCE_DIR})
set(InstallDir ${SolutionDir}/bin/${CMAKE_BUILD_TYPE})
set(CMAKE_INSTALL_PREFIX ${InstallDir})


message(STATUS "Project Dir = ${SolutionDir}")
message(STATUS "Install Dir = ${InstallDir}")


option(Debug "ON for Debug or OFF for release" ON)
if(Debug)
	set(CMAKE_BUILD_TYPE "Debug")
else()
	set(CMAKE_BUILD_TYPE "Release")
endif()
message(STATUS "Set build type to:" ${CMAKE_BUILD_TYPE})


if(CMAKE_SIZEOF_VOID_P MATCHES 8)
    set(PLATFORM 64)
    message(STATUS "Detected 64-bit platform")
else()
    set(PLATFORM 32)
    message(STATUS "Detected 32-bit platform")
endif()


set(WITH_SOURCE_TREE "true")
if(WIN32)
  include("${CMAKE_SOURCE_DIR}/cmake/win_settings.cmake")
elseif(UNIX)
  include("${CMAKE_SOURCE_DIR}/cmake/unix_settings.cmake")
endif()
include("${CMAKE_SOURCE_DIR}/cmake/macros/GroupSources.cmake")
include("${CMAKE_SOURCE_DIR}/cmake/macros/AutoCollect.cmake")


include_directories(
	${SolutionDir}/src
	${SolutionDir}/game
	${SolutionDir}/thirdparty/asio/asio/include
	${SolutionDir}/thirdparty/cinatra/include
	${SolutionDir}/thirdparty/nlohmann-json-3.11.2
	${SolutionDir}/thirdparty/async_simple-1.1
)

set(COMM_LINK_LIB common)

add_subdirectory(src)
add_subdirectory(game)


install(DIRECTORY  ${SolutionDir}/assets DESTINATION ${InstallDir})
install(FILES  
${SolutionDir}/assets/script/start_master.bat 
${SolutionDir}/assets/script/start_world.bat
DESTINATION ${InstallDir})




