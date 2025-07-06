#pragma once

#include <filesystem>
#include <fstream>

#include <nlohmann/json.hpp>

struct MainConfig
{
    std::string LOG_FILE;

    std::string MINECRAFT_LOG_FILE;
    std::string LAST_POS_FILE;
    std::string SERVER_URL;
    std::string SERVER_PORT;
    std::string MONTHS_PATH;
    std::string MINECRAFT_SERVER_IP;
    int         MINECRAFT_SERVER_PORT;
    std::string MINECRAFT_SERVER_PASSWORD;
    std::string SECRET_SERVER_PASSWORD;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(MainConfig, LOG_FILE, MINECRAFT_LOG_FILE, LAST_POS_FILE, SERVER_URL, SERVER_PORT, MONTHS_PATH, MINECRAFT_SERVER_IP, MINECRAFT_SERVER_PORT, MINECRAFT_SERVER_PASSWORD, SECRET_SERVER_PASSWORD)
};

inline MainConfig loadMainConfig(const std::string &config_path)
{
    std::ifstream config_file(config_path);
    auto config = nlohmann::json::parse(config_file);
    config_file.close();
    return config.template get<MainConfig>();
}