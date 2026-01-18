#ifndef INCLUDE_SOCKETHANDLER_H
#define INCLUDE_SOCKETHANDLER_H

#include <WebSocketsClient.h>
#include <functional>

/**
 * @brief Handles WebSocket connections for real-time updates
 */
class SocketHandler
{
private:
    WebSocketsClient* socket;

public:
    /**
     * @brief Construct a new Socket Handler object
     * @param callback Function to handle WebSocket events
     */
    explicit SocketHandler(std::function<void(WStype_t type, uint8_t* payload, size_t length)> callback);
    ~SocketHandler();
    
    /**
     * @brief Process WebSocket events (call in loop())
     */
    void loop();
};

#endif // INCLUDE_SOCKETHANDLER_H