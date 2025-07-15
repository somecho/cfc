set(SOYA_EXAMPLES
  basics
  basic-drawing
  fragment-shader
  framebuffer
  data-structures
  particles
  extras-lockfreequeue
  extras-pipeencoder
)

foreach(EXAMPLE IN ITEMS ${SOYA_EXAMPLES})
  add_executable(${EXAMPLE} ${EXAMPLE}.c)
  target_link_libraries(${EXAMPLE} PRIVATE soya)
  target_compile_definitions(${EXAMPLE} PUBLIC USE_CMAKE_SOYA)
endforeach()
