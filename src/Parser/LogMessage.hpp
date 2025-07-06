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

std::string toString(const LogType type)
{
    switch (type)
    {
        case LogType::message:      return "message";
        case LogType::left:         return "left";
        case LogType::join:         return "join";
        case LogType::achievement:  return "achievement";
        default:                    return "none"
    };
}

LogType toLogType(const std::string& str)
{
    if(str == "message")
        return LogType::message;
    if(str == "left")
        return LogType::left;
    if(str == "join")
        return LogType::join;
    if(str == "achievement")
        return LogType::achievement;
    return LogType::none; 
}

struct LogMessage
{
    LogType type = LogType::none;
    std::string playerName;
    std::string msg;

public:

    void clear()
    {
        type = LogType::none;
        playerName.clear();
        msg.clear();
    }

    bool parseFromString(const std::string& str)
    {
        clear();
        std::size_t pos{};
        if (pos = str.find("[Not Secure]"); pos != std::string::npos)
            return parseMsg(str, pos);
        if (pos = str.find("joined the game"); pos != std::string::npos)
            return parseJoin(str, pos);
        if (pos = str.find("left the game"); pos != std::string::npos)
            return parseLeft(str, pos);
        return false;
    }

private:

    bool parseMsg(const std::string& str, const std::size_t pos)
    {
        std::size_t begin = str.find('<', pos) + 1;
        std::size_t end = str.find('>', pos);
        playerName = str.substr(begin, end - begin);
        msg = str.substr(end + 2);
        type = LogType::message;
        return true;
    }
    bool parseJoin(const std::string& str, const std::size_t pos)
    {
        std::size_t begin = 0;
        for(begin = pos - 1; begin != 0 && str[begin - 1] != ' ';--begin);
        std::size_t end = pos - 1;
        playerName = str.substr(begin, end - begin);
        msg = "";
        type = LogType::join;
        return true;
    }
    bool parseLeft(const std::string& str, const std::size_t pos)
    {
        std::size_t begin = 0;
        for(begin = pos - 1; begin != 0 && str[begin - 1] != ' ';--begin);
        std::size_t end = pos - 1;
        playerName = str.substr(begin, end - begin);
        msg = "";
        type = LogType::left;
        return true;
    }

};