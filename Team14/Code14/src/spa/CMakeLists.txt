file(GLOB_RECURSE srcs "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE headers "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp")
add_library(spa ${srcs} ${headers} "src/sp/ast/node/simplenodetype.h" src/commons/parser/parser_exceptions.h src/sp/ast/grammar/basesimplegrammar.h src/sp/ast/grammar/earlychoicegrammar.h src/sp/ast/grammar/earlychoicegrammar.cpp src/sp/ast/grammar/listgrammar.h src/sp/ast/grammar/listgrammar.cpp src/sp/ast/grammar/recursivegrammar.h src/sp/ast/grammar/recursivegrammar.cpp)
# this makes the headers accessible for other projects which uses spa lib
target_include_directories(spa PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
