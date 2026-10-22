set(Sources example_test.cpp)

add_executable(mini_db_test ${Sources})

target_link_libraries(mini_db_test PRIVATE 
    mini_db
    gtest_main
)

include(GoogleTest)
gtest_discover_tests(mini_db_test)
