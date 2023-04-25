message(STATUS "CMake s2s_protocol")


CollectSourceFiles(${CMAKE_CURRENT_SOURCE_DIR} PRIVATE_SOURCES)
list(APPEND PRIVATE_SOURCES)
add_library(s2s_protocol ${PRIVATE_SOURCES})

GroupSources(${CMAKE_CURRENT_SOURCE_DIR})
CollectIncludeDirectories(${CMAKE_CURRENT_SOURCE_DIR} PUBLIC_INCLUDES)

set_target_properties(s2s_protocol PROPERTIES FOLDER "game")


