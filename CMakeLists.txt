cmake_minimum_required(VERSION 3.11)

project(soya
  VERSION 0.2.0
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

add_library(soya_glad ${CMAKE_CURRENT_SOURCE_DIR}/soya/glad/glad.c)
add_library(soya::glad ALIAS soya_glad)

add_library(soya INTERFACE)
add_library(soya::soya ALIAS soya)
target_include_directories(soya INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(soya INTERFACE ${SOYA_DEPENDENCIES} soya::glad)
if(UNIX)
target_compile_options(soya INTERFACE "$<$<CONFIG:Debug>:-g;-Wall;-Wpedantic;-D_DEBUG>")
target_compile_options(soya INTERFACE "$<$<CONFIG:Release>:-O3;-DNDEBUG;>")
endif()

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/examples)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/test)
