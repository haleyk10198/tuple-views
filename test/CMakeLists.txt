find_package(GTest REQUIRED)

include(GoogleTest)

add_executable(tuple_tests tuple_tests.cxx
        ../include/views/tuple_reverse.hxx)

target_link_libraries(tuple_tests range-v3::range-v3 GTest::gtest_main)
gtest_discover_tests(tuple_tests)
