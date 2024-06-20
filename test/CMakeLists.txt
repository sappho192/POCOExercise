project(tests)

add_executable(tests test.cpp)

# target_sources(tests PRIVATE test.cpp)
find_package(fmt CONFIG REQUIRED)
target_link_libraries(tests PRIVATE fmt::fmt-header-only)
set_property(TARGET tests PROPERTY CXX_STANDARD 23)
add_test(tests tests)