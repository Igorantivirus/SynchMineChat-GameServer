#pragma once

#include <thread>

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
    ~WebChatProcessor()
    {
        if(pollClientThPtr_ && pollClientThPtr_->joinable())
            pollClientThPtr_->join();
    }

    //main thread
    void run()
    {
        client_.connect("/ws");
        if(!client_.isConnected())
            reconnectToServer();
        pollClientThPtr_ = std::make_unique<std::thread>(&WebChatProcessor::otherThreadToWebSpcketPoll, this);

        chat_.start();

        while(isWorking_)
        {
            if(!chat_.isConnected())
            {
                Service::log.log("Connect to rcon is failed. ", LogLevel::Error);
                reconnectRcon();
            }
            auto& messanges = chat_.getNextMessages();
            while(!messanges.empty())
                sendMessageToServer(messanges.pop());
        }
    }

private:

    //Защищены мьютексом внутри
    MinecraftChatAPI chat_; 
    WebSocketClient client_;

    std::atomic<bool> isWorking_ = true;

    std::unique_ptr<std::thread> pollClientThPtr_ = nullptr;

private:

    // other thread
    void otherThreadToWebSpcketPoll()
    {
        while(isWorking_)
        {
            if(client_.isConnected())
                client_.poll();
        }
    }

    // other thread
    void reconnectToServer()
    {
        std::chrono::seconds downtime = std::chrono::seconds(0);
        while(!client_.isConnected())
        {
            Service::log.log("Try to reconnect to server with 5 seconds.", LogLevel::Error);
            client_.connect("/ws");
            std::this_thread::sleep_for(std::chrono::seconds(5));
            downtime += std::chrono::seconds(5);
            if(downtime >= std::chrono::seconds(30))
            {
                Service::log.log("The downtime was too long.", LogLevel::Critical);
                isWorking_ = false;
                return;
            }
        }
    }

    //this thread
    void reconnectRcon()
    {
        std::chrono::seconds downtime = std::chrono::seconds(0);
        while(!chat_.isConnected())
        {
            Service::log.log("Try to reconnect to rcon with 5 seconds.", LogLevel::Error);
            chat_.start();
            std::this_thread::sleep_for(std::chrono::seconds(5));
            downtime += std::chrono::seconds(5);
            if(downtime >= std::chrono::seconds(100))
            {
                Service::log.log("The simple one is too long", LogLevel::Critical);
                isWorking_ = false;
                return;
            }
        }
    }

private:

    // other thread
    void onConnect()
    {
        Service::log.log("Connect to server.", LogLevel::Info);
        nlohmann::json key;
        key["key"] = Service::config.SECRET_SERVER_PASSWORD;
        Service::log.log("Json: " + key.dump(), LogLevel::Debug);
        client_.sendMessage(key.dump());
    }

    // other thread
    void onDisconnect(websocket::close_code code)
    {
        Service::log.log("Connect to server is failed. ", LogLevel::Error);
        reconnectToServer();
    }

    // other thread
    void onMessage(const std::string& msg)
    {
        try
        {
            Message msg;
            msg.fromJson(nlohmann::json(msg));
            chat_.sendMessage(msg);
        }
        catch(...)
        {
            Service::log.log("A message has arrived from the server that is not in json format.", LogLevel::Error);
        }
    }

private:
    
    //main thread
    void sendMessageToServer(const LogMessage& msg)
    {        
        Message resMsg;
        
        resMsg.from = ClientType::minecraft;
        resMsg.to = ClientType::any;
        resMsg.type = msg.type;
        resMsg.msg["userName"] = msg.playerName;
        resMsg.msg["text"] = msg.msg;
        
        nlohmann::json json = resMsg.toJson();
        client_.sendMessage(json.dump());
    }

};