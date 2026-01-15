#ifndef INCLUDE_ERRORS_H
#define INCLUDE_ERRORS_H

/**
 * @brief Result codes for API operations
 */
enum class ApiResult
{
    SUCCESS,           // Operation completed successfully
    HTTP_ERROR,        // HTTP request failed
    JSON_PARSE_ERROR,  // JSON parsing failed
    NO_NETWORK,        // Not connected to network
    TIMEOUT,           // Request timed out
    INVALID_DATA,      // Response data invalid/incomplete
    UNKNOWN_ERROR      // Unspecified error
};

/**
 * @brief Result codes for initialization operations
 */
enum class InitResult
{
    SUCCESS,           // Initialization successful
    DISPLAY_FAILED,    // Display initialization failed
    WIFI_FAILED,       // WiFi connection failed
    TIME_SYNC_FAILED,  // NTP time sync failed
    COMPONENT_FAILED   // Component initialization failed
};

/**
 * @brief Convert ApiResult to human-readable string
 */
inline const char* apiResultToString(ApiResult result)
{
    switch (result)
    {
        case ApiResult::SUCCESS:          return "Success";
        case ApiResult::HTTP_ERROR:       return "HTTP Error";
        case ApiResult::JSON_PARSE_ERROR: return "JSON Parse Error";
        case ApiResult::NO_NETWORK:       return "No Network";
        case ApiResult::TIMEOUT:          return "Timeout";
        case ApiResult::INVALID_DATA:     return "Invalid Data";
        case ApiResult::UNKNOWN_ERROR:    return "Unknown Error";
        default:                          return "Unknown";
    }
}

/**
 * @brief Convert InitResult to human-readable string
 */
inline const char* initResultToString(InitResult result)
{
    switch (result)
    {
        case InitResult::SUCCESS:          return "Success";
        case InitResult::DISPLAY_FAILED:   return "Display Failed";
        case InitResult::WIFI_FAILED:      return "WiFi Failed";
        case InitResult::TIME_SYNC_FAILED: return "Time Sync Failed";
        case InitResult::COMPONENT_FAILED: return "Component Failed";
        default:                           return "Unknown";
    }
}

#endif // INCLUDE_ERRORS_H
