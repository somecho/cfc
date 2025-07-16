cmake_minimum_required(VERSION 3.11)

project(soya LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS true)
set(SOYA_DEPENDENCIES)

add_library(soya INTERFACE)
target_sources(soya INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/soya/glad/glad.c)
target_include_directories(soya INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})


include(FetchContent)

message(STATUS "Searching for glfw...")
find_package(glfw QUIET)
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

message(STATUS "Searching for cglm...")
find_package(cglm QUIET)
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

if(NOT WIN32)
  list(APPEND SOYA_DEPENDENCIES m)
endif()

target_link_libraries(soya INTERFACE ${SOYA_DEPENDENCIES})

option(SOYA_EXAMPLES "Build Soya Examples" OFF)
if(${SOYA_EXAMPLES})
message(STATUS "Soya Examples will be built")
# add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/examples)
set(SOYA_EXAMPLES
  basics
  basic-drawing
  fragment-shader
  framebuffer
  vectors
  particles
)

if(NOT WIN32)
  list(APPEND SOYA_EXAMPLES
    extras-lockfreequeue
    extras-pipeencoder
  )
endif()

foreach(EXAMPLE IN ITEMS ${SOYA_EXAMPLES})
  add_executable(${EXAMPLE} examples/${EXAMPLE}.c)
  target_link_libraries(${EXAMPLE} PRIVATE soya)
  target_compile_definitions(${EXAMPLE} PUBLIC USE_CMAKE_SOYA RESOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}/resources")
endforeach()

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
