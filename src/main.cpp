#ifndef _DEBUG

#define _DEBUG

#endif

#include "WebChatProcessor.hpp"

int main()
{
    Service::staticConstruct("resources/MainConfig.json");
    
    WebChatProcessor app;
    app.run();

    return 0;
}