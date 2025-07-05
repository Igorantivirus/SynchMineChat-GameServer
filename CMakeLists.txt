cmake_minimum_required(VERSION 3.16)

project(GameServer VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

#find_package(LibName CONFIG REQUIRED)

add_executable(main src/main.cpp)

# target_link_libraries(main PRIVATE LibName::lib...)