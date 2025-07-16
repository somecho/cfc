cmake_minimum_required(VERSION 3.11)

project(soya
  VERSION 0.1.0
  DESCRIPTION "Creative Coding in C"
  HOMEPAGE_URL https://github.com/somecho/soya
  LANGUAGES C
)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS true)

option(SOYA_EXAMPLES "Build Soya Examples" OFF)
option(SOYA_TESTS "Build Soya Tests" OFF)

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/FetchDependencies.cmake)


set(SOYA_DEPENDENCIES glfw cglm)
if(NOT WIN32)
  list(APPEND SOYA_DEPENDENCIES m)
endif()

add_library(soya INTERFACE)
add_library(soya::soya ALIAS soya)
target_sources(soya INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/soya/glad/glad.c)
target_include_directories(soya INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(soya INTERFACE ${SOYA_DEPENDENCIES})

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/examples)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/test)
