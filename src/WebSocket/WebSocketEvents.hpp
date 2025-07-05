#pragma once

#include <string>
#include <functional>

#include "Utils.hpp"

class WebSocketEvents
{
public:

    friend class WebSocketClient;

public:

    using OnConnectCallBack     = std::function<void()>;
    using OnDisconnecnCallBack  = std::function<void(websocket::close_code)>;
    using OnMessageCallBack     = std::function<void(const std::string&)>;

public:
    WebSocketEvents() = default;

    void onConnect(OnConnectCallBack onConnectF)
    {
        onConnectF_ = std::move(onConnectF);
    }
    void onClose(OnDisconnecnCallBack onCloseF)
    {
        onCloseF_ = std::move(onCloseF);
    }
    void onMessage(OnMessageCallBack onMessageF)
    {
        onMessageF_ = std::move(onMessageF);
    }

private:

    OnConnectCallBack onConnectF_ = nullptr;
    OnDisconnecnCallBack onCloseF_ = nullptr;
    OnMessageCallBack onMessageF_ = nullptr;

private:

    bool ready() const
    {
        return onConnectF_ && onCloseF_ && onMessageF_;
    }

};