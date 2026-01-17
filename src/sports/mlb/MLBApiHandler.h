#ifndef INCLUDE_MLB_APIHANDLER_H
#define INCLUDE_MLB_APIHANDLER_H

#include <Arduino.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "../../core/ApiHandler.h"
#include "MLBTeamInfo.h"

/**
 * @brief Handles HTTP requests to MLB Stats API
 */
class MLBApiHandler : public ApiHandler
{
private:
    HTTPClient http;
    DynamicJsonDocument doc;

public:
    MLBApiHandler();
    ~MLBApiHandler() override;
    
    /**
     * @brief Get team schedule for today
     * @param teamId MLB Team ID
     * @param outDoc Output document to populate with results
     * @return true on success, false on failure
     */
    bool getTeamScheduleToday(int teamId, DynamicJsonDocument& outDoc) override;
    
    /**
     * @brief Get schedule for a specific date
     * @param date Date in YYYY-MM-DD format
     * @param outDoc Output document to populate with results
     * @return true on success, false on failure
     */
    bool getSchedule(const String& date, DynamicJsonDocument& outDoc) override;
    
    /**
     * @brief Get team schedule for a specific date
     * @param teamId MLB Team ID
     * @param date Date in YYYY-MM-DD format
     * @param outDoc Output document to populate with results
     * @return true on success, false on failure
     */
    bool getTeamSchedule(int teamId, const String& date, DynamicJsonDocument& outDoc);
};

#endif // INCLUDE_MLB_APIHANDLER_H
