#pragma once

#include <boost/beast.hpp>
#include <boost/asio.hpp>

namespace beast     = boost::beast;
namespace websocket = boost::beast::websocket;
namespace net       = boost::asio;
using tcp           = boost::asio::ip::tcp;

