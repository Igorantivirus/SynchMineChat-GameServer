#pragma once

#include <filesystem>
#include <fstream>

#include <nlohmann/json.hpp>

struct MainConfig
{
    std::string LOG_FILE;

    std::string SERVER_URL;
    std::string SERVER_PORT;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(MainConfig, LOG_FILE, SERVER_URL, SERVER_PORT)
};

inline MainConfig loadMainConfig(const std::string &config_path)
{
    std::ifstream config_file(config_path);
    auto config = nlohmann::json::parse(config_file);
    config_file.close();
    return config.template get<MainConfig>();
}