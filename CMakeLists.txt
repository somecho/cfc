cmake_minimum_required(VERSION 3.11)

project(soya LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS true)
set(SOYA_DEPENDENCIES)

add_library(soya INTERFACE)
target_include_directories(soya INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})


include(FetchContent)

message(STATUS "Searching for glfw...")
find_package(glfw)
if(NOT ${glfw_FOUND})
  message(STATUS "Could not find glfw with find_package.")
  FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw
    GIT_TAG 3.4
    GIT_SHALLOW TRUE
  )
  list(APPEND SOYA_DEPENDENCIES glfw)
endif()

message(STATUS "Searching for libpng...")
find_package(png)
if(NOT ${png_FOUND})
  message(STATUS "Could not find libpng with find_package.")
  FetchContent_Declare(
    png
    GIT_REPOSITORY https://github.com/pnggroup/libpng
    GIT_TAG v1.6.50
    GIT_SHALLOW TRUE
  )
  list(APPEND SOYA_DEPENDENCIES png)
endif()

message(STATUS "Searching for libepoxy...")
find_package(epoxy)
if(NOT ${epoxy_FOUND})
  message(STATUS "Could not find libepoxy with find_package.")
  FetchContent_Declare(
    epoxy
    GIT_REPOSITORY https://github.com/anholt/libepoxy
    GIT_TAG 1.5.10
    GIT_SHALLOW TRUE
  )
  list(APPEND SOYA_DEPENDENCIES epoxy)
endif()
message(STATUS "Searching for libepoxy...")

message(STATUS "Searching for cglm...")
find_package(cglm)
if(NOT ${cglm_FOUND})
  message(STATUS "Could not find cglm with find_package.")
  FetchContent_Declare(
    cglm
    GIT_REPOSITORY https://github.com/recp/cglm
    GIT_TAG v0.9.6
    GIT_SHALLOW TRUE
  )
  list(APPEND SOYA_DEPENDENCIES cglm)
endif()

message(STATUS "Fetching dependencies: " ${SOYA_DEPENDENCIES})
FetchContent_MakeAvailable(${SOYA_DEPENDENCIES})

target_link_libraries(soya INTERFACE glfw epoxy png m)

option(SOYA_EXAMPLES "Build Soya Examples" OFF)
if(${SOYA_EXAMPLES})
message(STATUS "Soya Examples will be built")
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/examples)
else()
message(STATUS "Soya Examples will not be built. Configure with -DSOYA_EXAMPLES=ON to enable.")
endif()

option(SOYA_TESTS "Build Soya Tests" OFF)
if(${SOYA_TESTS})
message(STATUS "Soya Tests will be built")
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/test)
else()
message(STATUS "Soya Tests will not be built. Configure with -DSOYA_TESTS=ON to enable.")
endif()
