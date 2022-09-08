file(GLOB_RECURSE srcs "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE headers "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp")
add_executable(unit_testing ${srcs} src/sp/extractor/follows_test.cpp src/sp/extractor/parent_test.cpp src/sp/extractor/uses_test.cpp src/sp/extractor/modifies_test.cpp)

target_link_libraries(unit_testing spa gtest_main)

