#include <Arduino.h>
#include <WiFiManager.h>
#include <WebSocketsClient.h>
#include <AceButton.h>
#include "Config.h"
#include "TeamInfo.h"
#include "Game.h"
#include "ApiHandler.h"
#include "Matrix.h"
#include "GameDrawer.h"
#include "SocketHandler.h"
#include "touchButton.h"

// Global state
MatrixPanel_I2S_DMA* display = nullptr;
GameDrawer* gameDrawer = nullptr;
ApiHandler* apiHandler = nullptr;
SocketHandler* socketHandler = nullptr;
Game* currentGame = nullptr;

TEAM_ID currentTeam = TEAM_ID::BOSTON_REDSOX;
unsigned long lastUpdateTime = 0;

TouchButtonConfig touchConfig(T9);
ace_button::AceButton button(&touchConfig, T9);

// Function declarations
void updateScreen();
void buttonHandler(ace_button::AceButton* button, uint8_t eventType, uint8_t buttonState);
void handleSocketEvent(WStype_t type, uint8_t* payload, size_t length);
void wifiConnectionCallback(WiFiManager* wifiManager);

void updateScreen()
{
    lastUpdateTime = millis();
    
    DynamicJsonDocument schedule(ApiConfig::JSON_BUFFER_SIZE);
    bool success = apiHandler->getTeamScheduleToday(currentTeam, schedule);
    
    if (!success)
    {
        Serial.println("Failed to get schedule");
        return;
    }
    
    // Check if we have valid data
    if (!schedule["dates"].is<JsonArray>() || schedule["dates"].size() == 0)
    {
        Serial.println("No games scheduled for today");
        gameDrawer->drawFullscreenText("No game today");
        return;
    }
    
    if (!schedule["dates"][0]["games"].is<JsonArray>() || schedule["dates"][0]["games"].size() == 0)
    {
        Serial.println("No games in schedule");
        gameDrawer->drawFullscreenText("No game data");
        return;
    }
    
    JsonObject gameData = schedule["dates"][0]["games"][0].as<JsonObject>();
    
    // Delete old game if it exists
    if (currentGame != nullptr)
    {
        delete currentGame;
        currentGame = nullptr;
    }
    
    // Create new game
    currentGame = new Game(gameData);
    gameDrawer->drawGame(currentGame);
}

void handleSocketEvent(WStype_t type, uint8_t* payload, size_t length)
{
    currentTeam++;
    updateScreen();
    Serial.printf("Switched to team ID: %d\n", static_cast<int>(currentTeam));
}

void buttonHandler(ace_button::AceButton* btn, uint8_t eventType, uint8_t buttonState)
{
    switch (eventType)
    {
    case ace_button::AceButton::kEventPressed:
        Serial.println("Button pressed");
        currentTeam++;
        gameDrawer->drawFullscreenText("Next...");
        updateScreen();
        break;
    case ace_button::AceButton::kEventReleased:
        Serial.println("Button released");
        break;
    }
}

void wifiConnectionCallback(WiFiManager* wifiManager)
{
    Serial.println("Entering WiFi configuration mode");
    gameDrawer->drawWifi();
}

void setup()
{
    Serial.begin(115200);
    Serial.println("MLB Scoreboard starting...");
    
    // Configure NTP time
    configTime(NetworkConfig::GMT_OFFSET_SEC, NetworkConfig::DAYLIGHT_OFFSET_SEC, 
              NetworkConfig::NTP_SERVER);
    
    // Initialize display
    display = Matrix::getInstance();
    if (!display->begin())
    {
        Serial.println("ERROR: Matrix display initialization failed!");
        return;
    }
    
    display->setBrightness8(DisplayConfig::BRIGHTNESS);
    display->clearScreen();
    
    // Create game drawer
    gameDrawer = new GameDrawer(display);
    gameDrawer->drawLoading();
    
    // Configure touch button
    pinMode(AppConfig::BUTTON_PIN, INPUT);
    button.getButtonConfig()->setFeature(ace_button::ButtonConfig::kFeatureClick);
    button.setEventHandler(buttonHandler);
    
    // Connect to WiFi
    WiFiManager wm;
    wm.setAPCallback(wifiConnectionCallback);
    
    bool connected = wm.autoConnect(AppConfig::WIFI_AP_NAME);
    
    if (!connected)
    {
        Serial.println("Failed to connect to WiFi - restarting");
        gameDrawer->drawFullscreenText("WiFi Failed");
        delay(3000);
        ESP.restart();
    }
    
    Serial.println("Connected to WiFi");
    
    // Initialize API handler and WebSocket
    apiHandler = new ApiHandler();
    socketHandler = new SocketHandler(handleSocketEvent);
    
    // Initial screen update
    lastUpdateTime = -(AppConfig::UPDATE_INTERVAL_MS + 1);  // Force immediate update
    
    Serial.println("Setup complete");
}

void loop()
{
    // Process WebSocket events
    if (socketHandler)
    {
        socketHandler->loop();
    }
    
    // Check button
    button.check();
    
    // Update screen every interval
    if (millis() - lastUpdateTime > AppConfig::UPDATE_INTERVAL_MS)
    {
        updateScreen();
    }
}
