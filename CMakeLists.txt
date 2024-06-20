# CMakeLists.txt
cmake_minimum_required(VERSION 3.15)
project(poco_exercise)

# Find dependencies provided by vcpkg (via vcpkg.cmake)
# find_package(fmt CONFIG REQUIRED)

# main target
# add_executable(main)
# target_sources(main PRIVATE src/main.cpp)
# target_link_libraries(main PRIVATE fmt::fmt-header-only)
# set_property(TARGET main PROPERTY CXX_STANDARD 23)
add_subdirectory(src/example1_server example1_server)
add_subdirectory(src/example1_client example1_client)

# tests target
enable_testing()
add_subdirectory(test tests)