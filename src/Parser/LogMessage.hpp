#pragma once

#include "string"

#include "../MinecraftChat/SubUtility.hpp"

// enum class LogType : unsigned char
// {
//     none = 0,
//     text,
//     left,
//     join,
//     achievement
// };
// std::string toString(const LogType type)
// {
//     switch (type)
//     {
//         case LogType::text:         return "text";
//         case LogType::left:         return "left";
//         case LogType::join:         return "join";
//         case LogType::achievement:  return "achievement";
//         default:                    return "none";
//     }
// }
// LogType toLogType(const std::string& str)
// {
//     if(str == "text")
//         return LogType::text;
//     if(str == "left")
//         return LogType::left;
//     if(str == "join")
//         return LogType::join;
//     if(str == "achievement")
//         return LogType::achievement;
//     return LogType::none; 
// }

struct LogMessage
{
    MessageType type = MessageType::none;
    std::string playerName;
    std::string msg;

public:

    void clear()
    {
        type = MessageType::none;
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
        if (pos = str.find("has made the advancement"); pos != std::string::npos)
            return parseAchievement(str, pos);
        return false;
        //[16Jul2025 20:37:50.118] [Server thread/INFO] [net.minecraft.server.MinecraftServer/]: Igorantivirus has reached the goal [Р’РѕР·РґСѓС€РЅРѕРµ РјРіРЅРѕРјРµРЅРёРµ...]
    }

private:

    bool parseMsg(const std::string& str, const std::size_t pos)
    {
        std::size_t begin = str.find('<', pos) + 1;
        std::size_t end = str.find('>', pos);
        playerName = str.substr(begin, end - begin);
        msg = str.substr(end + 2);
        type = MessageType::text;
        return true;
    }
    bool parseJoin(const std::string& str, const std::size_t pos)
    {
        std::size_t begin = 0;
        for(begin = pos - 1; begin != 0 && str[begin - 1] != ' ';--begin);
        std::size_t end = pos - 1;
        playerName = str.substr(begin, end - begin);
        msg = "";
        type = MessageType::join;
        return true;
    }
    bool parseLeft(const std::string& str, const std::size_t pos)
    {
        std::size_t begin = 0;
        for(begin = pos - 1; begin != 0 && str[begin - 1] != ' ';--begin);
        std::size_t end = pos - 1;
        playerName = str.substr(begin, end - begin);
        msg = "";
        type = MessageType::left;
        return true;
    }
    bool parseAchievement(const std::string& str, const std::size_t pos)
    {
        std::size_t begin = 0;
        for(begin = pos - 1; begin != 0 && str[begin - 1] != ' ';--begin);
        std::size_t end = pos - 1;
        playerName = str.substr(begin, end - begin);
        
        begin = str.find('[', pos) + 1;
        end = str.find(']', begin);
        msg = str.substr(begin, end - begin);
        type = MessageType::achievement;

        return true;
    }
};