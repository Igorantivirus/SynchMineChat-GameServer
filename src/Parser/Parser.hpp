#pragma once

#include <string>
#include <exception>
#include <fstream>
#include <filesystem>
#include <queue>

#include "../Services/Service.hpp"

#include "LogMessage.hpp"

class MinecraftLogParser
{
public:
    MinecraftLogParser()
    {
        logFile_.open(Service::config.MINECRAFT_LOG_FILE, std::ios::in | std::ios::binary);
        if (!logFile_.is_open())
        {
            Service::log.log("Unable to open log file: " + Service::config.MINECRAFT_LOG_FILE, LogLevel::Error);
            throw std::logic_error{"Unable to open log file: " + Service::config.MINECRAFT_LOG_FILE};
        }
        logFile_.seekg(0, std::ios::end);
    }
    ~MinecraftLogParser()
    {
        logFile_.close();
    }

    void parse(std::queue<LogMessage> &queue)
    {   
        logFile_.clear();

        if(auto curentSize = std::filesystem::file_size(Service::config.MINECRAFT_LOG_FILE); logFile_.tellg() > curentSize)
            logFile_.seekg(0, std::ios::end);

        std::string line;
        LogMessage msg;

        while (getline(logFile_, line))
            if(msg.parseFromString(line))
                queue.push(msg);
    }

private:

    std::ifstream logFile_;

};
