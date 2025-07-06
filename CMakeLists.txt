cmake_minimum_required(VERSION 3.16)

project(GameServer VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
if(WIN32)
    set(RCON_LIBRARY_PATH "D:/dev/libraries/rconpp-main")
endif()

find_package(Boost 1.70 REQUIRED COMPONENTS system)
find_package(nlohmann_json 3.12.0 REQUIRED)

add_library(nlohmann_json INTERFACE IMPORTED)

add_executable(main
    src/main.cpp
    ${RCON_LIBRARY_PATH}/src/rconpp/client.cpp
    ${RCON_LIBRARY_PATH}/src/rconpp/utilities.cpp
)

if(WIN32)
    target_include_directories(main SYSTEM PUBLIC ${RCON_LIBRARY_PATH}/include/)
endif()

target_link_libraries(main 
    PRIVATE 
    Boost::boost
    Boost::system
    nlohmann_json::nlohmann_json
)