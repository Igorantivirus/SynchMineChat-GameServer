cmake_minimum_required(VERSION 3.16)

include(FetchContent)

project(GameServer VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

find_package(Boost 1.70 REQUIRED COMPONENTS system)
find_package(nlohmann_json REQUIRED)

FetchContent_Declare(
    rconpp
    GIT_REPOSITORY https://github.com/Jaskowicz1/rconpp
    GIT_TAG        main
)

FetchContent_MakeAvailable(rconpp)

add_executable(main src/main.cpp)

target_link_libraries(main 
    PRIVATE
    rconpp
    nlohmann_json
    Boost::boost
    Boost::system
)