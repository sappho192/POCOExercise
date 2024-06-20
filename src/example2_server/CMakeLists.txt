project(example2_server)

add_executable(example2_server main.cpp)

find_package(Poco REQUIRED COMPONENTS Foundation Net)

target_link_libraries(example2_server PRIVATE Poco::Foundation Poco::Net)
