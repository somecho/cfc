if(${SOYA_TESTS})
  message(STATUS "Soya Tests will be built")

  enable_testing()

  set(SOYA_TEST_FILES
    preprocessor
    vec
  )

  foreach(TEST IN ITEMS ${SOYA_TEST_FILES})
    add_executable(${TEST} ${CMAKE_CURRENT_SOURCE_DIR}/${TEST}.c)
    target_link_libraries(${TEST} PRIVATE soya_lib)
    target_compile_definitions(${TEST}
      PUBLIC
      USE_CMAKE_SOYA
      SOYA_TEST_RESOURCES="${CMAKE_CURRENT_SOURCE_DIR}/resources"
    )
    add_test(NAME ${TEST} COMMAND ${TEST})
  endforeach()

else()

  message(STATUS "Soya Tests will not be built. Configure with -DSOYA_TESTS=ON to enable.")

endif()
