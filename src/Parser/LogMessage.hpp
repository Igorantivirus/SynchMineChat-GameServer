#pragma once

#include "string"

enum class LogType : unsigned char
{
    none = 0,
    message,
    left,
    join,
    achievement
};

struct LogMessage
{
    LogType type;
    std::string playerName;
    std::string msg;
};