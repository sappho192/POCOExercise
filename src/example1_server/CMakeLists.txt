project(example1_server)

add_executable(example1_server main.cpp)

find_package(Poco REQUIRED COMPONENTS Foundation Net)

target_link_libraries(example1_server PRIVATE Poco::Foundation Poco::Net)
