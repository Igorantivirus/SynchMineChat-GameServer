#pragma once

#include <string>

#include <rconpp/rcon.h>

#include "../Services/Service.hpp"
#include "../AsynchContainers/AsynchQueue.hpp"
#include "../Parser/Parser.hpp"
#include "Messange.hpp"

class MinecraftChatAPI
{
public:
    MinecraftChatAPI() :
        rconClient_{Service::config.MINECRAFT_SERVER_IP, Service::config.MINECRAFT_SERVER_PORT, Service::config.MINECRAFT_SERVER_PASSWORD }
    {
        rconClient_.on_log = std::bind(&MinecraftChatAPI::onLog, this, std::placeholders::_1);
        onResponseF_ = std::bind(&MinecraftChatAPI::onResponse, this, std::placeholders::_1);
    }

    void sendMessage(const Message& message)
    {
        std::string text = '<' + message["userName"] + '>' + ' ' + message[text];
        std::string command = "/tellraw @a [\"" + text + "\"]";
        rconClient_.send_data(command, 3, rconpp::data_type::SERVERDATA_EXECCOMMAND, onResponseF_);
    }

    AsynchSafelyQueue<LogMessage>& getNextMessages()
    {
        parser_.parse(messages_);
        return messages_;
    }

private:

    MinecraftLogParser parser_;
    rconpp::rcon_client rconClient_;

    AsynchSafelyQueue<LogMessage> messages_;

private:
    
    std::function<void(const rconpp::response&)> onResponseF_;

    void onLog(const std::string_view& log)
    {
        Service::log.log("Message from RCON: " + std::string(log), LogLevel::Info);
    }

    void onResponse(const rconpp::response& response)
    {
        Service::log.log("Response on RCON send message: " + response.data, LogLevel::Info);
    }

};