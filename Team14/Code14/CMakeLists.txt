cmake_minimum_required(VERSION 3.20.2)
project(SPA)

set(CMAKE_VERBOSE_MAKEFILE on)

set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${PROJECT_SOURCE_DIR}/cmake")
message(STATUS "${PROJECT_SOURCE_DIR} ${PROJECT_BINARY_DIR}")
if ("${PROJECT_SOURCE_DIR}" STREQUAL "${PROJECT_BINARY_DIR}")
    message(FATAL_ERROR "In-source builds are forbidden.")
endif ()

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif ()

if (NOT MSVC)
    set(CMAKE_CXX_FLAGS_DEBUG "-g")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3 -fprofile-arcs -ftest-coverage")
endif ()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED on)

set(AUTOTESTER_ROOT "${CMAKE_CURRENT_LIST_DIR}/lib/autotester/${CMAKE_SYSTEM_PROCESSOR}/${CMAKE_SYSTEM_NAME}/${CMAKE_BUILD_TYPE}")

list(APPEND CMAKE_PREFIX_PATH "${AUTOTESTER_ROOT}")
find_package(autotester REQUIRED)
include_directories("${CMAKE_CURRENT_LIST_DIR}/lib")#include catch.hpp

add_subdirectory(src/spa)
add_subdirectory(src/autotester)
#add_subdirectory(src/autotester_gui)
add_subdirectory(src/unit_testing)
add_subdirectory(src/integration_testing)
add_subdirectory(src/runner)

Include(FetchContent)
FetchContent_Declare(
        spdlog
        GIT_REPOSITORY https://github.com/gabime/spdlog.git
        GIT_TAG v1.10.0
)
FetchContent_MakeAvailable(spdlog)

include(FetchContent)
FetchContent_Declare(
        googletest
        # Specify the commit you depend on and update it regularly.
        URL https://github.com/google/googletest/archive/5376968f6948923e2411081fd9372e71a59d8e77.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()
add_test(NAME unit_testing COMMAND unit_testing WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})
add_test(NAME integration_testing COMMAND integration_testing WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})
