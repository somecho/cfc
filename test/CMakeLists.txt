set(SOYA_TESTS
  preprocessor
)

enable_testing()
foreach(TEST IN ITEMS ${SOYA_TESTS})
  add_executable(${TEST} ${TEST}.c)
  target_link_libraries(${TEST} PRIVATE soya)
  target_compile_definitions(${TEST}
    PUBLIC
      USE_CMAKE_SOYA
      SOYA_TEST_RESOURCES="${CMAKE_CURRENT_SOURCE_DIR}/resources"
    )
  add_test(NAME ${TEST} COMMAND ${TEST})
endforeach()
