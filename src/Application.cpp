#include "Application.h"
#include "Matrix.h"
#include "NetworkManager.h"
#include <ArduinoJson.h>

// Global application instance for callbacks
Application* g_app = nullptr;

Application::Application()
    : display(nullptr)
    , currentSport(nullptr)
    , activeSportType(SportType::NHL)
    , socketHandler(nullptr)
    , touchConfig(nullptr)
    , button(nullptr)
    , lastUpdateTime(0)
    , customNoGameMessage("")
    , hasCustomNoGameMessage(false)
{
    // Components will be initialized in setup()
}

Application::~Application()
{
    // Clean up in reverse order of creation
    if (currentSport)
    {
        delete currentSport;
        currentSport = nullptr;
    }
    
    if (socketHandler)
    {
        delete socketHandler;
        socketHandler = nullptr;
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
        if (currentSport && currentSport->getDrawer())
        {
            currentSport->getDrawer()->drawFullscreenText("WiFi Failed");
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
    Serial.println("\n\n=== Scoreboard Starting ===");
    
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
    
    // Initialize network components and sport
    Serial.println("Initializing sport and network components...");
    currentSport = SportFactory::createSport(activeSportType, display);
    if (!currentSport)
    {
        Serial.println("ERROR: Failed to create sport");
        return false;
    }
    
    // Show loading on display
    if (currentSport->getDrawer())
    {
        currentSport->getDrawer()->drawLoading();
    }
    
    socketHandler = new SocketHandler(socketCallbackTrampoline);
    
    // Force initial update
    lastUpdateTime = -(AppConfig::UPDATE_INTERVAL_MS + 1);
    
    Serial.println("=== Setup Complete ===\n");
    return true;
}

void Application::updateScreen()
{
    if (!currentSport)
    {
        Serial.println("ERROR: Sport not initialized");
        return;
    }
    
    lastUpdateTime = millis();
    
    String sportName = SportFactory::sportTypeToString(activeSportType);
    Serial.printf("Updating screen for %s...\n", sportName.c_str());
    
    // Sport handles its own update logic (API, game creation, drawing)
    currentSport->update();
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

void Application::switchSport(SportType newSport)
{
    if (newSport == activeSportType) return;
    
    Serial.printf("Switching from %s to %s\n",
                  SportFactory::sportTypeToString(activeSportType).c_str(),
                  SportFactory::sportTypeToString(newSport).c_str());
    
    // Clean up old sport
    if (currentSport)
    {
        delete currentSport;
        currentSport = nullptr;
    }
    
    // Create new sport
    currentSport = SportFactory::createSport(newSport, display);
    activeSportType = newSport;
    
    if (!currentSport)
    {
        Serial.println("ERROR: Failed to create new sport");
        return;
    }
    
    // Force immediate update
    updateScreen();
}

void Application::handleButtonEvent(uint8_t eventType, uint8_t buttonState)
{
    switch (eventType)
    {
    case ace_button::AceButton::kEventPressed:
        Serial.println("Button pressed - switching team");
        if (currentSport)
        {
            if (currentSport->getDrawer())
            {
                currentSport->getDrawer()->drawFullscreenText("Next...");
            }
            currentSport->nextTeam();
            updateScreen();
        }
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
            
            if (command == "switchSport")
            {
                // Switch sport
                if (!doc.containsKey("data"))
                {
                    Serial.println("switchSport command missing 'data' field");
                    return;
                }
                
                String sportName = doc["data"].as<String>();
                SportType newSport = SportFactory::sportTypeFromString(sportName);
                switchSport(newSport);
            }
            else if (command == "nextTeam")
            {
                // Cycle to next team within current sport
                if (currentSport)
                {
                    currentSport->nextTeam();
                    updateScreen();
                }
            }
            else if (command == "newMessage")
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
                
                // Update display immediately if we have a drawer
                if (currentSport && currentSport->getDrawer())
                {
                    currentSport->getDrawer()->drawFullscreenText(customNoGameMessage);
                }
            }
            else if (command == "clearMessage")
            {
                // Clear custom message
                customNoGameMessage = "";
                hasCustomNoGameMessage = false;
                
                Serial.println("Custom message cleared");
                
                // Update screen to resume normal game display
                updateScreen();
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
    if (currentSport && currentSport->getDrawer())
    {
        currentSport->getDrawer()->drawWifi();
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
