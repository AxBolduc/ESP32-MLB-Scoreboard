#ifndef INCLUDE_APIHANDLER_H
#define INCLUDE_APIHANDLER_H

#include <Arduino.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "TeamInfo.h"

/**
 * @brief Handles HTTP requests to MLB Stats API
 */
class ApiHandler
{
private:
    HTTPClient http;
    DynamicJsonDocument doc;

public:
    ApiHandler();
    ~ApiHandler();
    
    /**
     * @brief Get team schedule for today
     * @param team Team ID
     * @param outDoc Output document to populate with results
     * @return true on success, false on failure
     */
    bool getTeamScheduleToday(TEAM_ID team, DynamicJsonDocument& outDoc);
    
    /**
     * @brief Get schedule for a specific date
     * @param date Date in YYYY-MM-DD format
     * @param outDoc Output document to populate with results
     * @return true on success, false on failure
     */
    bool getSchedule(const String& date, DynamicJsonDocument& outDoc);
    
    /**
     * @brief Get team schedule for a specific date
     * @param team Team ID
     * @param date Date in YYYY-MM-DD format
     * @param outDoc Output document to populate with results
     * @return true on success, false on failure
     */
    bool getTeamSchedule(TEAM_ID team, const String& date, DynamicJsonDocument& outDoc);
};

#endif // INCLUDE_APIHANDLER_H
