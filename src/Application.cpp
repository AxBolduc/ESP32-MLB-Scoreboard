#include "Application.h"
#include "Matrix.h"
#include "NetworkManager.h"
#include <ArduinoJson.h>

// Global application instance for callbacks
Application* g_app = nullptr;

Application::Application()
    : display(nullptr)
    , gameDrawer(nullptr)
    , apiHandler(nullptr)
    , socketHandler(nullptr)
    , touchConfig(nullptr)
    , button(nullptr)
    , currentGame(nullptr)
    , currentTeam(TEAM_ID::BOSTON_REDSOX)
    , lastUpdateTime(0)
    , customNoGameMessage("")
    , hasCustomNoGameMessage(false)
{
    // Components will be initialized in setup()
}

Application::~Application()
{
    // Clean up in reverse order of creation
    if (currentGame)
    {
        delete currentGame;
        currentGame = nullptr;
    }
    
    if (socketHandler)
    {
        delete socketHandler;
        socketHandler = nullptr;
    }
    
    if (apiHandler)
    {
        delete apiHandler;
        apiHandler = nullptr;
    }
    
    if (button)
    {
        delete button;
        button = nullptr;
    }
    
    if (touchConfig)
    {
        delete touchConfig;
        touchConfig = nullptr;
    }
    
    if (gameDrawer)
    {
        delete gameDrawer;
        gameDrawer = nullptr;
    }
    
    // Don't delete display - it's managed by Matrix singleton
}

void Application::configureTime()
{
    Serial.println("Configuring NTP time...");
    NetworkManager::configureTime(NetworkConfig::NTP_SERVER,
                                  NetworkConfig::GMT_OFFSET_SEC,
                                  NetworkConfig::DAYLIGHT_OFFSET_SEC);
}

void Application::initializeDisplay()
{
    Serial.println("Initializing display...");
    
    display = Matrix::getInstance();
    if (!display->begin())
    {
        Serial.println("ERROR: Matrix display initialization failed!");
        return;
    }
    
    display->setBrightness8(DisplayConfig::BRIGHTNESS);
    display->clearScreen();
    
    gameDrawer = new GameDrawer(display);
    gameDrawer->drawLoading();
    
    Serial.println("Display initialized");
}

bool Application::connectToWiFi()
{
    Serial.println("Connecting to WiFi...");
    
    bool connected = NetworkManager::connectToWiFi(AppConfig::WIFI_AP_NAME, 
                                                   wifiCallbackTrampoline);
    
    if (!connected)
    {
        Serial.println("Failed to connect to WiFi");
        if (gameDrawer)
        {
            gameDrawer->drawFullscreenText("WiFi Failed");
        }
        return false;
    }
    
    Serial.println("Connected to WiFi");
    return true;
}

void Application::setupButton()
{
    Serial.println("Configuring button...");
    
    pinMode(AppConfig::BUTTON_PIN, INPUT);
    
    touchConfig = new TouchButtonConfig(T9);
    button = new ace_button::AceButton(touchConfig, T9);
    
    button->getButtonConfig()->setFeature(ace_button::ButtonConfig::kFeatureClick);
    button->setEventHandler(buttonCallbackTrampoline);
    
    Serial.println("Button configured");
}

bool Application::setup()
{
    Serial.begin(115200);
    Serial.println("\n\n=== MLB Scoreboard Starting ===");
    
    // Set global pointer for callbacks
    g_app = this;
    
    // Configure time first
    configureTime();
    
    // Initialize display
    initializeDisplay();
    if (!display)
    {
        Serial.println("FATAL: Display initialization failed");
        return false;
    }
    
    // Setup button
    setupButton();
    
    // Connect to WiFi
    if (!connectToWiFi())
    {
        return false;
    }
    
    // Initialize network components
    Serial.println("Initializing network components...");
    apiHandler = new ApiHandler();
    socketHandler = new SocketHandler(socketCallbackTrampoline);
    
    // Force initial update
    lastUpdateTime = -(AppConfig::UPDATE_INTERVAL_MS + 1);
    
    Serial.println("=== Setup Complete ===\n");
    return true;
}

void Application::updateScreen()
{
    if (!apiHandler || !gameDrawer)
    {
        Serial.println("ERROR: Components not initialized");
        return;
    }
    
    lastUpdateTime = millis();
    
    Serial.printf("Updating screen for team %d...\n", static_cast<int>(currentTeam));
    
    DynamicJsonDocument schedule(ApiConfig::JSON_BUFFER_SIZE);
    bool success = apiHandler->getTeamScheduleToday(currentTeam, schedule);
    
    if (!success)
    {
        Serial.println("Failed to get schedule");
        return;
    }
    
    // Validate response structure
    if (!schedule["dates"].is<JsonArray>() || schedule["dates"].size() == 0)
    {
        Serial.println("No games scheduled for today");
        if (hasCustomNoGameMessage)
        {
            gameDrawer->drawFullscreenText(customNoGameMessage);
        }
        else
        {
            gameDrawer->drawFullscreenText("No game today");
        }
        return;
    }
    
    if (!schedule["dates"][0]["games"].is<JsonArray>() || 
        schedule["dates"][0]["games"].size() == 0)
    {
        Serial.println("No games in schedule");
        if (hasCustomNoGameMessage)
        {
            gameDrawer->drawFullscreenText(customNoGameMessage);
        }
        else
        {
            gameDrawer->drawFullscreenText("No game data");
        }
        return;
    }
    
    JsonObject gameData = schedule["dates"][0]["games"][0].as<JsonObject>();
    
    // Delete old game if exists
    if (currentGame)
    {
        delete currentGame;
        currentGame = nullptr;
    }
    
    // Create and display new game
    currentGame = new Game(gameData);
    gameDrawer->drawGame(currentGame);
    
    Serial.println("Screen updated successfully");
}

void Application::loop()
{
    // Process WebSocket events
    if (socketHandler)
    {
        socketHandler->loop();
    }
    
    // Check button
    if (button)
    {
        button->check();
    }
    
    // Periodic screen update
    if (millis() - lastUpdateTime > AppConfig::UPDATE_INTERVAL_MS)
    {
        updateScreen();
    }
}

// Internal event handlers

void Application::handleButtonEvent(uint8_t eventType, uint8_t buttonState)
{
    switch (eventType)
    {
    case ace_button::AceButton::kEventPressed:
        Serial.println("Button pressed - switching team");
        currentTeam++;
        if (gameDrawer)
        {
            gameDrawer->drawFullscreenText("Next...");
        }
        updateScreen();
        break;
        
    case ace_button::AceButton::kEventReleased:
        Serial.println("Button released");
        break;
    }
}

void Application::handleSocketEvent(WStype_t type, uint8_t* payload, size_t length)
{

    Serial.println("Handling socket event");

    switch (type)
    {
    case WStype_DISCONNECTED:
        Serial.println("WebSocket disconnected");
        break;
        
    case WStype_CONNECTED:
        Serial.println("WebSocket connected");
        break;
        
    case WStype_TEXT:
        Serial.printf("WebSocket message received: %s\n", payload);
        
        // Parse JSON message
        {
            DynamicJsonDocument doc(512);
            DeserializationError error = deserializeJson(doc, payload, length);
            
            if (error)
            {
                Serial.printf("JSON parse error: %s\n", error.c_str());
                return;
            }
            
            // Check for command field
            if (!doc.containsKey("command"))
            {
                Serial.println("WebSocket message missing 'command' field");
                return;
            }
            
            String command = doc["command"].as<String>();
            
            if (command == "newMessage")
            {
                // Set custom message
                if (!doc.containsKey("data"))
                {
                    Serial.println("newMessage command missing 'data' field");
                    return;
                }
                
                customNoGameMessage = doc["data"].as<String>();
                hasCustomNoGameMessage = true;
                
                Serial.printf("Custom message set: %s\n", customNoGameMessage.c_str());
                
                // Update display immediately if we're currently showing a no-game message
                if (gameDrawer)
                {
                    gameDrawer->drawFullscreenText(customNoGameMessage);
                }
            }
            else if (command == "clearMessage")
            {
                // Clear custom message
                customNoGameMessage = "";
                hasCustomNoGameMessage = false;
                
                Serial.println("Custom message cleared");
                
                // Update display immediately to show default message
                if (gameDrawer)
                {
                    gameDrawer->drawFullscreenText("No game today");
                }
            }
            else
            {
                Serial.printf("Unknown command: %s\n", command.c_str());
            }
        }
        break;
        
    default:
        break;
    }
}

void Application::handleWiFiAPMode()
{
    Serial.println("Entering WiFi AP configuration mode");
    if (gameDrawer)
    {
        gameDrawer->drawWifi();
    }
}

// Static callback trampolines

void Application::buttonCallbackTrampoline(ace_button::AceButton* btn, uint8_t eventType, uint8_t buttonState)
{
    if (g_app)
    {
        g_app->handleButtonEvent(eventType, buttonState);
    }
}

void Application::socketCallbackTrampoline(WStype_t type, uint8_t* payload, size_t length)
{
    if (g_app)
    {
        g_app->handleSocketEvent(type, payload, length);
    }
}

void Application::wifiCallbackTrampoline(WiFiManager* wm)
{
    if (g_app)
    {
        g_app->handleWiFiAPMode();
    }
}
