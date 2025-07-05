#pragma once

#include "Logger.hpp"
#include "Configs/MainConfig.hpp"

class Service
{
public:
    static MainConfig config;
    static ServerLogger log;

    static void staticConstruct(const std::string &configFileName)
    {
        config = loadMainConfig(configFileName);
#ifdef _DEBUG
        log.setEntryToConsole(true);
#endif
        log.openFile(config.LOG_FILE);
    }
};

MainConfig Service::config;
ServerLogger Service::log;