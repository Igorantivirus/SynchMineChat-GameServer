#pragma once

#include <utility>
#include <string>
#include <mutex>
#include <atomic>
#include <functional>

#include <boost/beast.hpp>
#include <boost/asio.hpp>

#include "../Services/Service.hpp"

#include "Utils.hpp"
#include "WebSocketEvents.hpp"

class WebSocketClient
{
public:
    WebSocketClient() : ws_{io_}
    {
        ws_.set_option(websocket::stream_base::timeout::suggested(beast::role_type::client));
        ws_.auto_fragment(true);

        pollCallSettings_.first = false;
        // pollCallSettings_.second = std::this_thread::get_id();
    }
    ~WebSocketClient()
    {
        disconnect();
    }

    WebSocketEvents& getEvents()
    {
        return events_;
    }

    void setSettings(const std::string& ip, const std::string& port)
    {
        if(pollCallSettings_.first)
            throw std::logic_error("You cannot force a change of settings in the callback functions when \"poll\" is running.");
        // if(pollCallSettings_.first && pollCallSettings_.second == std::this_thread::get_id())
        //     throw std::logic_error("You cannot force a change of settings in the callback functions when \"poll\" is running.");
        settings_.ip = ip;
        settings_.port = port;
        settings_.results = tcp::resolver(io_).resolve(ip, port);
    }

    bool connect(const std::string &servPath)
    {
        try
        {
            net::connect(ws_.next_layer(), settings_.results);
            ws_.handshake(settings_.ip, servPath);
        }
        catch (const beast::system_error &e)
        {
            isConnected_ = false;
            Service::log.log("Failed to connect to server!", LogLevel::Error);
            return false;
        }
        isConnected_ = true;
        readSetAsyncTask();
        if(events_.onConnectF_)
            events_.onConnectF_();
        Service::log.log("Sucsess connect to server!", LogLevel::Info);
        return true;
    }

    void disconnect(websocket::close_code code = websocket::close_code::normal)
    {
        if (!isConnected_)
            return;
        isConnected_ = false;
        try
        {
            ws_.close(code);
        }
        catch (...)
        {}
        if(events_.onCloseF_)
            events_.onCloseF_(code);
        Service::log.log("Disconnect from server!", LogLevel::Info);
    }

    bool isConnected() const
    {
        return isConnected_;
    }

    void poll()
    {
        // if(!isConnected_)
        //     return;
        // if(pollCallSettings_.first && pollCallSettings_.second == std::this_thread::get_id())
        //     throw std::logic_error("It is forbidden to call \"poll\" from callback functions.");

        if(pollCallSettings_.first)
            throw std::logic_error("It is forbidden to call \"poll\" from callback functions.");

        pollCallSettings_.first = true;

        io_.poll();

        pollCallSettings_.first = false;
    }

    void sendMessage(const std::string& msg)
    {
        ws_.async_write(net::buffer(msg), [this](beast::error_code ec, std::size_t s){asyncWriteCallBack(ec, s);});
    }
    
    void ping()
    {
        ws_.async_ping("", [this](beast::error_code ec) {asyncPingCallBack(ec);});
    }

private:

    struct WebSocketSettings
    {
        std::string port;
        std::string ip;
        net::ip::basic_resolver_results<tcp> results; 
    };

private:

    using SettingsT = std::pair<std::string, std::string>;

    std::pair<bool, std::thread::id> pollCallSettings_{};

    net::io_context io_;
    websocket::stream<tcp::socket> ws_;
    WebSocketSettings settings_;

    bool isConnected_ = false;

    beast::flat_buffer readBuffer;
    WebSocketEvents events_;

private:

    void readSetAsyncTask()
    {
        ws_.async_read(readBuffer, [this](beast::error_code ec, size_t s){asyncReadCallBeck(ec, s);});
    }

    void asyncPingCallBack(beast::error_code ec)
    {
        if (ec)
            disconnect(websocket::close_code::protocol_error); 
    }

    void asyncWriteCallBack(beast::error_code ec, std::size_t)
    {
        if (ec)
            disconnect(websocket::close_code::internal_error);
    }

    void asyncReadCallBeck(beast::error_code ec, size_t)
    {
        std::string messageFromServer = beast::buffers_to_string(readBuffer.data());
        readBuffer.clear();
        if (!ec)
        {
            events_.onMessageF_(messageFromServer);
            if(isConnected_)
                readSetAsyncTask();  // Следующий цикл чтения
        }
        else
            disconnect(websocket::close_code::internal_error);
    }

};