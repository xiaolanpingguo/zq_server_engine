message(STATUS "CMake db_mongo")


CollectSourceFiles(${CMAKE_CURRENT_SOURCE_DIR} PRIVATE_SOURCES)
list(APPEND PRIVATE_SOURCES)
add_library(db_mongo ${PRIVATE_SOURCES})

GroupSources(${CMAKE_CURRENT_SOURCE_DIR})
CollectIncludeDirectories(${CMAKE_CURRENT_SOURCE_DIR} PUBLIC_INCLUDES)

set_target_properties(db_mongo PROPERTIES FOLDER "game")


