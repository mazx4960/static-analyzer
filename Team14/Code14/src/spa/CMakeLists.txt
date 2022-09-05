file(GLOB_RECURSE srcs "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE headers "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp")
add_library(spa ${srcs} ${headers} "src/sp/ast/simplenodetype.h" src/commons/parser/parser_exceptions.h src/sp/grammar/basesimplegrammar.h src/sp/grammar/earlychoicegrammar.h src/sp/grammar/earlychoicegrammar.cpp src/sp/grammar/listgrammar.h src/sp/grammar/listgrammar.cpp src/sp/grammar/recursivegrammar.h src/sp/grammar/recursivegrammar.cpp src/sp/grammar/latechoicegrammar.cpp src/sp/grammar/latechoicegrammar.h src/sp/grammar/grammarproducer.cpp src/sp/grammar/grammarproducer.h)
# this makes the headers accessible for other projects which uses spa lib
target_include_directories(spa PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
