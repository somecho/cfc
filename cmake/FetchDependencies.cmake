include(FetchContent)

message(STATUS "Looking for glfw")
find_package(glfw QUIET)
if(NOT ${glfw_FOUND})
  message(STATUS "Looking for glfw - not found")
  set(SOYA_GLFW_VERSION 3.4)
  FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw
    GIT_TAG ${SOYA_GLFW_VERSION}
    GIT_SHALLOW TRUE
  )
  message(STATUS "Fetching glfw ${SOYA_GLFW_VERSION}")
  FetchContent_MakeAvailable(glfw)
endif()

message(STATUS "Looking for cglm")
find_package(cglm QUIET)
if(NOT ${cglm_FOUND})
  message(STATUS "Looking for cglm - not found")
  set(SOYA_CGLM_VERSION v0.9.6)
  FetchContent_Declare(
    cglm
    GIT_REPOSITORY https://github.com/recp/cglm
    GIT_TAG ${SOYA_CGLM_VERSION}
    GIT_SHALLOW TRUE
  )
  message(STATUS "Fetching cglm ${SOYA_CGLM_VERSION}")
  FetchContent_MakeAvailable(cglm)
endif()
