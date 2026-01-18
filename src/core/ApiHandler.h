#ifndef INCLUDE_CORE_APIHANDLER_H
#define INCLUDE_CORE_APIHANDLER_H

#include <Arduino.h>
#include <ArduinoJson.h>

/**
 * @brief Abstract base class for sport-specific API handlers
 * 
 * Each sport implementation provides its own API handler that knows how to
 * fetch schedule and game data from the appropriate sports API.
 */
class ApiHandler
{
public:
    virtual ~ApiHandler() = default;

    virtual bool getTeamGame(String teamAbr, DynamicJsonDocument& outDoc) = 0;
    
    /**
     * @brief Get team schedule for today
     * @param teamId Sport-specific team ID
     * @param outDoc Output document to populate with results
     * @return true on success, false on failure
     */
    virtual bool getTeamScheduleToday(int teamId, DynamicJsonDocument& outDoc) = 0;
    
    /**
     * @brief Get schedule for a specific date
     * @param date Date in YYYY-MM-DD format
     * @param outDoc Output document to populate with results
     * @return true on success, false on failure
     */
    virtual bool getSchedule(const String& date, DynamicJsonDocument& outDoc) = 0;
};

#endif // INCLUDE_CORE_APIHANDLER_H
