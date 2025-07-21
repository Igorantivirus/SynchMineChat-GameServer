#pragma once

#include "Parser/LogMessage.hpp"

#include "MinecraftChat/MinecraftChatAPI.hpp"
#include "WebSocket/WebSocketClient.hpp"

class WebChatProcessor
{
public:
    WebChatProcessor()
    {
        client_.getEvents().onClose(std::bind(&WebChatProcessor::onDisconnect, this, std::placeholders::_1)); 
        client_.getEvents().onMessage(std::bind(&WebChatProcessor::onMessage, this, std::placeholders::_1));
        client_.getEvents().onConnect(std::bind(&WebChatProcessor::onConnect, this));

        client_.setSettings(Service::config.SERVER_URL, Service::config.SERVER_PORT);
    }

    void run()
    {
        connectaAll();

        while(isWorking_)
        {
            reconnectToServer();
            reconnectToRcon();
            auto& messanges = chat_.getNextMessages();
            while(!messanges.empty())
            {
                sendMessageToServer(messanges.front());
                messanges.pop();
            }
            client_.poll();
            std::this_thread::sleep_for(std::chrono::milliseconds(64));
        }
    }

private:

    MinecraftChatAPI chat_; 
    WebSocketClient client_;

    bool isWorking_ = true;

private:

    void connectaAll()
    {
        Service::log.log("Try to connect to server.", LogLevel::Info);
        client_.connect("/ws");
        if(!client_.isConnected())
            reconnectToServer();
        else
            Service::log.log("Sucsessful connect to server.", LogLevel::Info);

        Service::log.log("Try to connect to rcon.", LogLevel::Info);
        chat_.start();
        if(!chat_.isConnected())
            reconnectToRcon();
        else
            Service::log.log("Sucsessful connect to rcon.", LogLevel::Info);
    }

    void otherThreadToWebSpcketPoll()
    {
        while(isWorking_)
        {
            if(client_.isConnected())
                client_.poll();
        }
    }

    void reconnectToServer()
    {
        if(client_.isConnected() && isWorking_)
            return;
        Service::log.log("Server connect fatal.", LogLevel::Error);
        std::chrono::seconds downtime = std::chrono::seconds(0);
        while(!client_.isConnected())
        {
            std::this_thread::sleep_for(std::chrono::seconds(5));
            downtime += std::chrono::seconds(5);
            if(downtime >= std::chrono::seconds(50))
            {
                Service::log.log("The downtime was too long.", LogLevel::Critical);
                isWorking_ = false;
                return;
            }
            Service::log.log("Try to reconnect to server with 5 seconds.", LogLevel::Error);
            client_.connect("/ws");
        }
    }

    void reconnectToRcon()
    {
        if(chat_.isConnected() && isWorking_)
            return;
        Service::log.log("Rcon connect fatal.", LogLevel::Error);
        std::chrono::seconds downtime = std::chrono::seconds(0);
        while(!chat_.isConnected())
        {
            std::this_thread::sleep_for(std::chrono::seconds(5));
            downtime += std::chrono::seconds(5);
            if(downtime >= std::chrono::seconds(50))
            {
                Service::log.log("The downtime was too long.", LogLevel::Critical);
                isWorking_ = false;
                return;
            }
            Service::log.log("Try to reconnect to server with 5 seconds.", LogLevel::Error);
            chat_.start();
        }
    }

private:

    void onConnect()
    {
        Service::log.log("Connect to server.", LogLevel::Info);
        nlohmann::json key;
        key["key"] = Service::config.SECRET_SERVER_PASSWORD;
        client_.sendMessage(key.dump());
    }

    void onDisconnect(websocket::close_code code)
    {
        Service::log.log("Connect to server is failed. ", LogLevel::Error);
        reconnectToServer();
    }

    void onMessage(const std::string& str)
    {
        try
        {
            Message realMes;
            realMes.fromJson(toJson(str));
            chat_.sendMessage(realMes);
        }
        catch(...)
        {
            Service::log.log("A message has arrived from the server that is not in json format.", LogLevel::Error);
        }
    }

private:
    
    static nlohmann::json toJson(const std::string& str)
    {
        try
        {
            return nlohmann::json::parse(str);
        }
        catch(...)
        {
            return nlohmann::json{};
        }
    }

    void sendMessageToServer(const LogMessage& msg)
    {        
        Message resMsg;
        
        resMsg.from = ClientType::minecraft;
        resMsg.to = ClientType::any;
        resMsg.type = msg.type;
        resMsg.msg["userName"] = msg.playerName;
        resMsg.msg["text"] = msg.msg;
        
        nlohmann::json json = resMsg.toJson();
        client_.sendMessage(std::move(json.dump()));
    }

};