set(SOYA_EXAMPLES
  basics
  basic-drawing
  fragment-shader
  framebuffer
  data-structures
  particles
)

if(NOT WIN32)
  list(APPEND SOYA_EXAMPLES
    extras-lockfreequeue
    extras-pipeencoder
  )
endif()

foreach(EXAMPLE IN ITEMS ${SOYA_EXAMPLES})
  add_executable(${EXAMPLE} ${EXAMPLE}.c)
  target_link_libraries(${EXAMPLE} PRIVATE soya)
  target_compile_definitions(${EXAMPLE} PUBLIC USE_CMAKE_SOYA)
endforeach()
