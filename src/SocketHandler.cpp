#include "SocketHandler.h"
#include "Config.h"

SocketHandler::SocketHandler(std::function<void(WStype_t type, uint8_t* payload, size_t length)> callback)
{
    socket = new WebSocketsClient();
    socket->begin(NetworkConfig::WEBSOCKET_URL, 80, "/ws?deviceId=display");
    socket->onEvent(callback);
}

SocketHandler::~SocketHandler()
{
    if (socket)
    {
        socket->disconnect();
        delete socket;
        socket = nullptr;
    }
}

void SocketHandler::loop()
{
    if (socket)
    {
        socket->loop();
    }
}
