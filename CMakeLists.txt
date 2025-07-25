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
option(SOYA_CORE "Include soya::core to use as framework" ON)

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/FetchDependencies.cmake)

set(SOYA_DEPENDENCIES glfw cglm)
if(NOT WIN32)
  list(APPEND SOYA_DEPENDENCIES m)
endif()


add_library(soya_lib ${CMAKE_CURRENT_SOURCE_DIR}/soya/lib/lib.c)
target_include_directories(soya_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
if(NOT WIN32)
  target_link_libraries(soya_lib PUBLIC m)
endif()
add_library(soya::lib ALIAS soya_lib)

if(${SOYA_CORE})
  add_library(soya_core ${CMAKE_CURRENT_SOURCE_DIR}/soya/core/core.c)
  target_include_directories(soya_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
  add_library(soya::core ALIAS soya_core)

  add_library(soya_glad ${CMAKE_CURRENT_SOURCE_DIR}/soya/glad/glad.c)
  add_library(soya::glad ALIAS soya_glad)

  add_library(soya INTERFACE)
  add_library(soya::soya ALIAS soya)
  target_include_directories(soya INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
  target_link_libraries(soya INTERFACE ${SOYA_DEPENDENCIES} soya::glad soya::lib soya::core)

  if(UNIX)
    target_compile_options(soya INTERFACE "$<$<CONFIG:Debug>:-g;-Wall;-Wpedantic;-D_DEBUG>")
    target_compile_options(soya INTERFACE "$<$<CONFIG:Release>:-O3;-DNDEBUG;>")
  endif()
endif()

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/test)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/examples)
