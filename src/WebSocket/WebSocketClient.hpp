#pragma once

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
    }
    ~WebSocketClient()
    {
        disconnect();
    }

    WebSocketEvents& getEvents()
    {
        return events_;
    }

    bool connect(const std::string &ip, const std::string &port, const std::string &servPath)
    {
        std::lock_guard lg1(wsMut_);
        std::lock_guard lg2(ioMut_);
        try
        {
            auto const results = tcp::resolver(io_).resolve(ip, port);
            net::connect(ws_.next_layer(), results);
            ws_.handshake(ip, servPath);
            isConnected_ = true;
            readSetAsyncTask();
            events_.onConnectF_();
            Service::log.log("Sucsess connect to server!", LogLevel::Info);
            return true;
        }
        catch (const beast::system_error &e)
        {
            isConnected_ = false;
            Service::log.log("Failed to connect to server!", LogLevel::Error);
            return false;
        }
    }

    void disconnect(websocket::close_code code = websocket::close_code::normal)
    {
        std::lock_guard lg1(wsMut_);
        if (!isConnected_)
            return;
        try
        {
            ws_.close(code);
        }
        catch (...)
        {}
        events_.onCloseF_(code);
        Service::log.log("Disconnect from server!", LogLevel::Info);
        isConnected_ = false;
    }

    bool isConnected() const
    {
        return isConnected_;
    }

    void poll()
    {
        std::lock_guard lg1(ioMut_);
        if(isConnected_)
            io_.poll();
    }

    void sendMessage(const std::string& msg)
    {
        std::lock_guard lg1(wsMut_);
        ws_.async_write(net::buffer(msg), [this](beast::error_code ec, std::size_t s){asyncWriteCallBack(ec, s);});
    }

    void ping()
    {
        std::lock_guard lg1(wsMut_);
        ws_.async_ping("", [this](beast::error_code ec) {asyncPingCallBack(ec);});
    }

private:
    net::io_context io_;
    websocket::stream<tcp::socket> ws_;
    
    std::mutex ioMut_;
    std::mutex wsMut_;

    std::atomic<bool> isConnected_ = false;

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
        if (!ec)
        {
            events_.onMessageF_(beast::buffers_to_string(readBuffer.data()));
            if(isConnected_)
                readSetAsyncTask();  // Следующий цикл чтения
        }
        else
            disconnect(websocket::close_code::internal_error);//Просто закроем соединение
        readBuffer.consume(readBuffer.size()); // Очищаем буфер
    }

};