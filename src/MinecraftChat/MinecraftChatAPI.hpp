#pragma once

#include <string>
#include <queue>

#include <rconpp/rcon.h>

#include "../Services/Service.hpp"
#include "../Parser/Parser.hpp"
#include "Messange.hpp"
#include "TellrawGenerator.hpp"

class MinecraftChatAPI
{
public:
    MinecraftChatAPI() :
        rconClient_{Service::config.MINECRAFT_SERVER_IP, Service::config.MINECRAFT_SERVER_PORT, Service::config.MINECRAFT_SERVER_PASSWORD }
    {
        rconClient_.on_log = std::bind(&MinecraftChatAPI::onLog, this, std::placeholders::_1);
        onResponseF_ = std::bind(&MinecraftChatAPI::onResponse, this, std::placeholders::_1);
    }

    bool isConnected() const
    {
        return rconClient_.connected;
    }

    bool start()
    {
        try
        {
            rconClient_.start(true);
        }
        catch(const std::exception& e)
        {
            Service::log.log(std::string("Std error from rcon start: ") + e.what(), LogLevel::Error);
        }
        catch(...)
        {
            Service::log.log("Error to start rcon.", LogLevel::Error);
        }
        return rconClient_.connected;
    }

    void sendMessage(const Message& message)
    {
        if(message.type == MessageType::text)
            sendText(message);
        else if(message.type == MessageType::media)
            sendMedia(message);
        else if(message.type == MessageType::audio)
            sendAudio(message);
    }

    std::queue<LogMessage>& getNextMessages()
    {
        parser_.parse(messages_);
        return messages_;
    }

private:

    MinecraftLogParser parser_;
    rconpp::rcon_client rconClient_;

    std::queue<LogMessage> messages_;

    TellrawGenerator generator_;

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

private:

    void sendText(const Message& message)
    {
        std::string command = generator_.baseMessage(message["userName"], message["text"]);
        rconClient_.send_data(command, 3, rconpp::data_type::SERVERDATA_EXECCOMMAND, onResponseF_);   
    }
    void sendMedia(const Message& message)
    {
        std::string command = generator_.mediaMessage(message["userName"]);
        rconClient_.send_data(command, 3, rconpp::data_type::SERVERDATA_EXECCOMMAND, onResponseF_);   
    }
    void sendAudio(const Message& message)
    {
        std::string command = generator_.audioMessage(message["userName"]);
        rconClient_.send_data(command, 3, rconpp::data_type::SERVERDATA_EXECCOMMAND, onResponseF_);   
    }

};