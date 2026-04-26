include(GoogleTest)

function(add_feature_test name)
  add_executable(${name} ${name}.cpp)
  target_link_libraries(${name}
    PRIVATE
      cpp-modern-lib
      GTest::gtest_main
  )
  target_compile_features(${name} PRIVATE cxx_std_20)
  gtest_discover_tests(${name})
endfunction()

add_feature_test(test_spaceship_operator)
add_feature_test(test_concepts)
add_feature_test(test_ranges)