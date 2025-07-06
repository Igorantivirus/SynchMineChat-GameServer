#include "WebSocket/WebSocketClient.hpp"

std::string input(std::string str = {})
{
    std::cout << str;
    std::getline(std::cin, str);

    return str;
}

void onMessage(const std::string& str)
{
    std::cout << "From server: " << str << '\n';
}
void onConnect()
{
    std::cout << "Connect: ";
}
void onDisconnect(websocket::close_code)
{
    std::cout << "Disconnect\n";
}

#include <nlohmann/json.hpp>

#include "Parser/Utils.hpp"

#include "MinecraftChat/MinecraftChatAPI.hpp"

#include "Parser/Parser.hpp"

int main()
{
    Service::staticConstruct("resources/MainConfig.json");

    MinecraftLogParser parser;
    AsynchSafelyQueue<LogMessage> queue;

    parser.parse(queue);

    while(!queue.empty())
    {
        LogMessage msg = queue.pop();
        // std::cout << std::format
        std::cout << toString(msg.type) << ' ' << msg.playerName << ' ' << msg.msg << '\n';   
    }


    // DateTime dt = DateTime::fromLogString("[05Jul2025 08:33:37.817]");
    // std::cout << dt.date << ' ' << dt.time << '\n';
    // std::cout << DateTime::toLogString(dt) << '\n';

    // 
    // Service::log.setEntryToConsole(true);

    // const constexpr int s = sizeof("[05Jul2025 08:33:37.817]");

    // Service::log.log("My info", LogLevel::Critical);

    // WebSocketClient wb;
    // wb.getEvents().onMessage(onMessage);
    // wb.getEvents().onClose(onDisconnect);
    // wb.getEvents().onConnect(onConnect);

    // wb.connect("localhost", "18080", "/ws");

    // std::thread t([&]()
    // {
    //     while(wb.isConnected())
    //         wb.poll();
    // });

    // while(wb.isConnected())
    // {   
    //     wb.sendMessage(input("Input: "));
    // }


    // t.join();

    return 0;
}