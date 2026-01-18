#include <Arduino.h>
#include "Application.h"

// Global application instance
Application* app = nullptr;

void setup()
{
    // Create application
    app = new Application();
    
    // Initialize application
    if (!app->setup())
    {
        Serial.println("FATAL: Application setup failed!");
        Serial.println("Restarting in 5 seconds...");
        delay(5000);
        ESP.restart();
    }
}

void loop()
{
    if (app)
    {
        app->loop();
    }
}
