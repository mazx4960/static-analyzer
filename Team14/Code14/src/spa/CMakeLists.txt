file(GLOB_RECURSE srcs "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE headers "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp")
add_library(spa ${srcs} ${headers} src/sp/extractor/exception.h src/sp/extractor/entity_extractor.h src/sp/extractor/entity_extractor.cpp src/sp/extractor/extractor.h src/sp/extractor/relationship_extractor.cpp src/sp/extractor/relationship_extractor.h)
# this makes the headers accessible for other projects which uses spa lib
target_include_directories(spa PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
