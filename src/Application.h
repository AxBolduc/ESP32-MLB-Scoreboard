#ifndef INCLUDE_APPLICATION_H
#define INCLUDE_APPLICATION_H

#include <Arduino.h>
#include <WiFiManager.h>
#include <WebSocketsClient.h>
#include <AceButton.h>
#include <ESP32-HUB75-MatrixPanel-I2S-DMA.h>
#include "Config.h"
#include "core/Sport.h"
#include "core/SportFactory.h"
#include "SocketHandler.h"
#include "touchButton.h"

/**
 * @brief Main application orchestrator - owns all components and state
 * 
 * Encapsulates all global state and coordinates between display, networking,
 * and input handling. Now uses Sport abstraction for multi-sport support.
 */
class Application
{
private:
    // Display components
    MatrixPanel_I2S_DMA* display;
    
    // Sport management (replaces individual gameDrawer, apiHandler, currentGame, team)
    Sport* currentSport;
    SportType activeSportType;
    
    // Network components
    SocketHandler* socketHandler;
    
    // Input handling
    TouchButtonConfig* touchConfig;
    ace_button::AceButton* button;
    
    // Application state
    unsigned long lastUpdateTime;
    String customNoGameMessage;      // Custom message to show when no game
    bool hasCustomNoGameMessage;     // Flag indicating custom message is set
    
    // Private methods
    void updateScreen();
    void initializeDisplay();
    bool connectToWiFi();
    void configureTime();
    void setupButton();
    void switchSport(SportType newSport);
    
    // Callback handlers (internal)
    void handleButtonEvent(uint8_t eventType, uint8_t buttonState);
    void handleSocketEvent(WStype_t type, uint8_t* payload, size_t length);
    void handleWiFiAPMode();

public:
    /**
     * @brief Construct a new Application object
     */
    Application();
    
    /**
     * @brief Destroy the Application object and clean up resources
     */
    ~Application();
    
    /**
     * @brief Initialize the application
     * @return true if initialization successful, false otherwise
     */
    bool setup();
    
    /**
     * @brief Main application loop - call repeatedly
     */
    void loop();
    
    // Static callback trampolines (required for C-style callbacks)
    static void buttonCallbackTrampoline(ace_button::AceButton* btn, uint8_t eventType, uint8_t buttonState);
    static void socketCallbackTrampoline(WStype_t type, uint8_t* payload, size_t length);
    static void wifiCallbackTrampoline(WiFiManager* wm);
    
    // Allow trampolines to access private members
    friend void buttonCallbackTrampoline(ace_button::AceButton*, uint8_t, uint8_t);
    friend void socketCallbackTrampoline(WStype_t, uint8_t*, size_t);
    friend void wifiCallbackTrampoline(WiFiManager*);
};

// Global application instance pointer (for callbacks)
extern Application* g_app;

#endif // INCLUDE_APPLICATION_H
