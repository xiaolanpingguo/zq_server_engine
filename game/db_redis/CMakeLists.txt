message(STATUS "CMake db_redis")


CollectSourceFiles(${CMAKE_CURRENT_SOURCE_DIR} PRIVATE_SOURCES)
list(APPEND PRIVATE_SOURCES)
add_library(db_redis ${PRIVATE_SOURCES})

GroupSources(${CMAKE_CURRENT_SOURCE_DIR})
CollectIncludeDirectories(${CMAKE_CURRENT_SOURCE_DIR} PUBLIC_INCLUDES)

set_target_properties(db_redis PROPERTIES FOLDER "game")


