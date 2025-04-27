#ifndef INCLUDE_SOCKETHANDLER_H
#define INCLUDE_SOCKETHANDLER_H

#include <WebSocketsClient.h>
#include "constants.h"

class SocketHandler
{
private:
    WebSocketsClient *socket;
public:
    SocketHandler(std::function<void(WStype_t type, uint8_t * payload, size_t length)>  callback);
    ~SocketHandler();
    void loop();
};

SocketHandler::SocketHandler(std::function<void(WStype_t type, uint8_t * payload, size_t length)>  callback)
{
    this->socket = new WebSocketsClient();

    socket->begin(WEBSOCKET_URL , 80, "/ws");
    socket->onEvent(callback);
}

SocketHandler::~SocketHandler()
{
    this->socket->disconnect();
    delete this->socket;
}

void SocketHandler::loop() {
    this->socket->loop();
}

#endif