cmake_minimum_required(VERSION 3.16)

include(FetchContent)

project(GameServer VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

find_package(Boost 1.70 REQUIRED COMPONENTS system)
find_package(nlohmann_json 3.12.0 REQUIRED)

add_library(nlohmann_json INTERFACE IMPORTED)

FetchContent_Declare(
    rconpp
    GIT_REPOSITORY https://github.com/Jaskowicz1/rconpp
    GIT_TAG        main
)

FetchContent_MakeAvailable(rconpp)

add_executable(main src/main.cpp)

target_link_libraries(main PRIVATE rconpp)
target_link_libraries(main 
    PRIVATE 
    Boost::boost
    Boost::system
    nlohmann_json::nlohmann_json
)