#pragma once

#include <string>
#include <exception>
#include <fstream>
#include <filesystem>
#include <limits>
#include <queue>

#include "../Services/Service.hpp"

#include "LogMessage.hpp"

class MinecraftLogParser
{
public:
    MinecraftLogParser()
    {
        outLastPosFile((std::numeric_limits<std::streamsize>::max)());
        openLogFile();
    }

    void parse(std::queue<LogMessage> &queue)
    {
        logFile_.clear();
        logFile_.seekg(lastPos_);
        
        std::string line;
        LogMessage msg;
        while (getline(logFile_, line))
            if(msg.parseFromString(line))
                queue.push(msg);

        outLastPosFile(std::filesystem::file_size(Service::config.MINECRAFT_LOG_FILE));
    }

private:

    std::ifstream logFile_;
    std::streamsize lastPos_ = (std::numeric_limits<std::streamsize>::max)();

    void openLogFile()
    {
        logFile_.open(Service::config.MINECRAFT_LOG_FILE, std::ios::in | std::ios::binary);
        if (!logFile_.is_open())
        {
            Service::log.log("Unable to open log file: " + Service::config.MINECRAFT_LOG_FILE, LogLevel::Error);
            throw std::logic_error("Unable to open log file: ");
        }
        const std::streamsize fileSize = std::filesystem::file_size(Service::config.MINECRAFT_LOG_FILE);
        const std::streamsize lastSize = getLastPos();

        lastPos_ = lastSize < fileSize ? lastSize : fileSize;
    }
    
    void outLastPosFile(const std::streamsize pos)
    {
        lastPos_ = pos;
        std::ofstream out(Service::config.LAST_POS_FILE);
        out << pos;
        out.close();
    }
    
    std::streamsize getLastPos()
    {
        std::ifstream in{Service::config.LAST_POS_FILE};
        if(!in.is_open())
            return (std::numeric_limits<std::streamsize>::max)();
        std::streamsize res;
        in >> res;
        in.close();
        return res;
    }
};
