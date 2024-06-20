project(example2_client)

add_executable(example2_client main.cpp)

find_package(Poco REQUIRED COMPONENTS Foundation Net)

target_link_libraries(example2_client PRIVATE Poco::Foundation Poco::Net)