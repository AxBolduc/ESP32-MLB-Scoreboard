#ifndef INCLUDE_NETWORKMANAGER_H
#define INCLUDE_NETWORKMANAGER_H

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiManager.h>

/**
 * @brief Handles network connectivity (WiFi and NTP time synchronization)
 */
class NetworkManager
{
public:
    /**
     * @brief Connect to WiFi network
     * @param apName Access Point name to use if connection fails
     * @param apCallback Callback when entering AP mode
     * @return true if connected, false otherwise
     */
    static bool connectToWiFi(const char* apName, 
                             std::function<void(WiFiManager*)> apCallback = nullptr);
    
    /**
     * @brief Configure NTP time synchronization
     * @param ntpServer NTP server address
     * @param gmtOffset GMT offset in seconds
     * @param dstOffset DST offset in seconds
     */
    static void configureTime(const char* ntpServer, long gmtOffset, int dstOffset);
    
    /**
     * @brief Check if WiFi is connected
     * @return true if connected, false otherwise
     */
    static bool isConnected();
    
    /**
     * @brief Get local IP address
     * @return IP address as string
     */
    static String getLocalIP();
    
    /**
     * @brief Get WiFi signal strength
     * @return RSSI value in dBm
     */
    static int getSignalStrength();

private:
    NetworkManager() = delete;  // Static class, no instantiation
    NetworkManager(const NetworkManager&) = delete;
    NetworkManager& operator=(const NetworkManager&) = delete;
};

#endif // INCLUDE_NETWORKMANAGER_H
