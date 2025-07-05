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

int main()
{
    Service::log.setEntryToConsole(true);

    WebSocketClient wb;
    wb.getEvents().onMessage(onMessage);
    wb.getEvents().onClose(onDisconnect);
    wb.getEvents().onConnect(onConnect);

    wb.connect("localhost", "18080", "/ws");

    std::thread t([&]()
    {
        while(wb.isConnected())
            wb.poll();
    });

    while(wb.isConnected())
    {   
        wb.sendMessage(input("Input: "));
    }


    t.join();

    return 0;
}