#ifndef INCLUDE_CONFIG_H
#define INCLUDE_CONFIG_H

/**
 * @brief Display configuration constants
 */
struct DisplayConfig
{
    static const int PANEL_WIDTH = 64;
    static const int PANEL_HEIGHT = 32;
    static const int PANELS_NUMBER = 1;
    static const int DISPLAY_WIDTH = 64;   // Total display width
    static const int BRIGHTNESS = 32;
    static const int BOX_PADDING = 2;
    
    // Base positioning and sizing
    static const int BASE_SIDE_LENGTH = 4;
    static const int BASES_TOP_X = 32;
    static const int BASES_TOP_Y = 11;
    
    // Text wrapping configuration
    static const int TEXT_LINE_SPACING = 2;  // Pixels between lines
    static const int TEXT_MAX_LINES = 4;     // Maximum lines to display
};

/**
 * @brief Network and API configuration constants
 */
struct NetworkConfig
{
    static const char* BASE_URL;
    static const char* WEBSOCKET_URL;
    static const char* NTP_SERVER;
    static const long GMT_OFFSET_SEC = -5 * 3600;  // EST (-5 hours)
    static const int DAYLIGHT_OFFSET_SEC = 3600;    // DST offset
};

/**
 * @brief API configuration constants
 */
struct ApiConfig
{
    static const int JSON_BUFFER_SIZE = 4096;
    static const int HYDRATIONS_ARRAY_SIZE = 32;
    static const int FIELDS_ARRAY_SIZE = 32;
};

/**
 * @brief Application behavior configuration
 */
struct AppConfig
{
    static const unsigned long UPDATE_INTERVAL_MS = 30000;  // 30 seconds
    static const int BUTTON_PIN = 32;
    static const int TOUCH_THRESHOLD = 35;
    static const char* WIFI_AP_NAME;
    static const int WIFI_TIMEOUT_SECONDS = 180;
};

#endif // INCLUDE_CONFIG_H
