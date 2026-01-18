#include "NetworkManager.h"
#include "Config.h"

bool NetworkManager::connectToWiFi(const char* apName, std::function<void(WiFiManager*)> apCallback)
{
    Serial.print("Connecting to WiFi");
    
    WiFiManager wm;
    
    if (apCallback)
    {
        wm.setAPCallback(apCallback);
    }
    
    bool connected = wm.autoConnect(apName);
    
    if (connected)
    {
        Serial.println(" - Connected!");
        Serial.print("IP Address: ");
        Serial.println(WiFi.localIP());
    }
    else
    {
        Serial.println(" - Failed");
    }
    
    return connected;
}

void NetworkManager::configureTime(const char* ntpServer, long gmtOffset, int dstOffset)
{
    Serial.println("Configuring NTP time synchronization...");
    configTime(gmtOffset, dstOffset, ntpServer);
    
    // Wait a moment for time to sync
    delay(1000);
    
    struct tm timeInfo;
    if (getLocalTime(&timeInfo))
    {
        Serial.printf("Time synchronized: %04d-%02d-%02d %02d:%02d:%02d\n",
                     timeInfo.tm_year + 1900, timeInfo.tm_mon + 1, timeInfo.tm_mday,
                     timeInfo.tm_hour, timeInfo.tm_min, timeInfo.tm_sec);
    }
    else
    {
        Serial.println("Warning: Time synchronization may have failed");
    }
}

bool NetworkManager::isConnected()
{
    return WiFi.status() == WL_CONNECTED;
}

String NetworkManager::getLocalIP()
{
    if (isConnected())
    {
        return WiFi.localIP().toString();
    }
    return "Not connected";
}

int NetworkManager::getSignalStrength()
{
    if (isConnected())
    {
        return WiFi.RSSI();
    }
    return 0;
}
