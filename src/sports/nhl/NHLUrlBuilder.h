#ifndef INCLUDE_NHL_URLBUILDER_H
#define INCLUDE_NHL_URLBUILDER_H

#include <Arduino.h>
#include <vector>

/**
 * @brief Builds URLs for NHL API endpoints
 * 
 * NOTE: This is currently stubbed. When NHL API integration is ready,
 * update with actual NHL API endpoints and URL structure.
 */
class NHLUrlBuilder
{
public:
    /**
     * @brief Build a schedule URL for a specific date
     * @param date Date in YYYY-MM-DD format
     * @return Complete URL string (currently stubbed)
     */
    static String buildScheduleUrl(const String& date);
    
    /**
     * @brief Build a schedule URL for a specific team and date
     * @param date Date in YYYY-MM-DD format
     * @param teamId NHL team ID
     * @return Complete URL string (currently stubbed)
     */
    static String buildTeamScheduleUrl(const String& date, int teamId);
    
    /**
     * @brief Get today's date in YYYY-MM-DD format
     * @return Date string or empty string on failure
     */
    static String getTodayDate();

private:
    NHLUrlBuilder() = delete;  // No instantiation
    NHLUrlBuilder(const NHLUrlBuilder&) = delete;
    NHLUrlBuilder& operator=(const NHLUrlBuilder&) = delete;
};

#endif // INCLUDE_NHL_URLBUILDER_H
