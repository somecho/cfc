if(${SOYA_EXAMPLES})

  message(STATUS "Soya Examples will be built")
  set(SOYA_EXAMPLE_FILES)

  if(${SOYA_CORE})
    list(APPEND SOYA_EXAMPLE_FILES
      basics
      basic-drawing
      3d-drawing
      fragment-shader
      framebuffer
      particles
      sysl
    )
    if(NOT WIN32)
      list(APPEND SOYA_EXAMPLE_FILES extras-pipeencoder)
    endif()
  endif()

  list(APPEND SOYA_EXAMPLE_FILES vectors)

  if(NOT WIN32)
    list(APPEND SOYA_EXAMPLE_FILES extras-lockfreequeue)
  endif()

  foreach(EXAMPLE IN ITEMS ${SOYA_EXAMPLE_FILES})
    add_executable(${EXAMPLE} ${CMAKE_CURRENT_SOURCE_DIR}/${EXAMPLE}.c)

    if(${SOYA_CORE})
      target_link_libraries(${EXAMPLE} PRIVATE soya::soya)
    else()
      target_link_libraries(${EXAMPLE} PRIVATE soya::lib)
    endif()

    target_compile_definitions(${EXAMPLE}
      PUBLIC
      USE_CMAKE_SOYA
      RESOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}/resources"
    )
  endforeach()

else()

  message(STATUS "Soya Examples will not be built. Configure with -DSOYA_EXAMPLES=ON to enable.")

endif()
