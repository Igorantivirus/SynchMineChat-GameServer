# SynchMineChat-GameServer
## (Synchronous Minecraft Chat — Game Server)

[![Russian version](https://img.shields.io/badge/Russian%20version-blue)](README_RU.md)

## Description

A Minecraft log parser that sends logs to a server via WebSocket protocol. 
Processes in-game chat messages in real-time and can capture:
1) Text messages
2) Player join messages
3) Player leave messages
4) Achievement unlocks by players

The main server can be found here: [Main Server Link](https://github.com/Igorantivirus/SynchMineChat-MainServer)

## Dependencies

Programming language: **C++20**

Libraries:
* [RconPP](https://github.com/JBakamovic/rconpp) - RCON client for C++
* [Boost](https://www.boost.org/) (version 1.75+)
  * [Asio](https://www.boost.org/doc/libs/release/doc/html/boost_asio.html) - for asynchronous I/O
  * [WebSocket](https://www.boost.org/doc/libs/release/libs/beast/doc/html/beast/using_websocket.html) - WebSocket implementation
* [nlohmann-json](https://github.com/nlohmann/json) - JSON library

## Implementation Features

* If there's no connection to the server or RCON for an extended period (default: 50 seconds) - the program terminates
* When the Minecraft log file updates, it only reads new messages
* Runs in a single thread
* Supports configuration via JSON files

## Installation and Running

### Prerequisites
* C++20 compatible compiler (GCC 10+, Clang 10+, MSVC 19.29+)
* CMake 3.15+
* Installed dependencies (see above)

### Build Instructions

1. **Clone the repository**
   ```sh
   git clone https://github.com/Igorantivirus/SynchMineChat-GameServer
   cd SynchMineChat-GameServer
   ```
