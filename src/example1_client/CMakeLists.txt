project(example1_client)

add_executable(example1_client main.cpp)

find_package(Poco REQUIRED COMPONENTS Foundation Net)

target_link_libraries(example1_client PRIVATE Poco::Foundation Poco::Net)