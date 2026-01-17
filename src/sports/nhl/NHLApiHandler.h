#ifndef INCLUDE_NHL_APIHANDLER_H
#define INCLUDE_NHL_APIHANDLER_H

#include <Arduino.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "../../core/ApiHandler.h"
#include "NHLTeamInfo.h"

/**
 * @brief Handles HTTP requests to NHL API
 * 
 * NOTE: This is currently stubbed and returns no game data.
 * When NHL API integration is ready, implement the actual API calls.
 */
class NHLApiHandler : public ApiHandler
{
private:
    HTTPClient http;
    DynamicJsonDocument doc;

public:
    NHLApiHandler();
    ~NHLApiHandler() override;
    
    /**
     * @brief Get team schedule for today (STUBBED)
     * @param teamId NHL Team ID
     * @param outDoc Output document to populate with results
     * @return false (no game data available yet)
     */
    bool getTeamScheduleToday(int teamId, DynamicJsonDocument& outDoc) override;
    
    /**
     * @brief Get schedule for a specific date (STUBBED)
     * @param date Date in YYYY-MM-DD format
     * @param outDoc Output document to populate with results
     * @return false (no game data available yet)
     */
    bool getSchedule(const String& date, DynamicJsonDocument& outDoc) override;
    
    /**
     * @brief Get team schedule for a specific date (STUBBED)
     * @param teamId NHL Team ID
     * @param date Date in YYYY-MM-DD format
     * @param outDoc Output document to populate with results
     * @return false (no game data available yet)
     */
    bool getTeamSchedule(int teamId, const String& date, DynamicJsonDocument& outDoc);
};

#endif // INCLUDE_NHL_APIHANDLER_H
