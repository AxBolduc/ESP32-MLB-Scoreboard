#ifndef INCLUDE_MLB_URLBUILDER_H
#define INCLUDE_MLB_URLBUILDER_H

#include <Arduino.h>
#include <vector>

/**
 * @brief Builds URLs for MLB Stats API endpoints
 */
class MLBUrlBuilder
{
public:
    /**
     * @brief Build a schedule URL for a specific date
     * @param date Date in YYYY-MM-DD format
     * @return Complete URL string
     */
    static String buildScheduleUrl(const String& date);
    
    /**
     * @brief Build a schedule URL for a specific team and date
     * @param date Date in YYYY-MM-DD format
     * @param teamId MLB team ID
     * @return Complete URL string
     */
    static String buildTeamScheduleUrl(const String& date, int teamId);
    
    /**
     * @brief Get today's date in YYYY-MM-DD format
     * @return Date string or empty string on failure
     */
    static String getTodayDate();

private:
    MLBUrlBuilder() = delete;  // No instantiation
    MLBUrlBuilder(const MLBUrlBuilder&) = delete;
    MLBUrlBuilder& operator=(const MLBUrlBuilder&) = delete;
    
    /**
     * @brief Internal helper to build API URLs
     */
    static String buildApiUrl(
        const String& endpoint,
        const std::vector<String>& hydrations,
        const std::vector<String>& fields
    );
};

#endif // INCLUDE_MLB_URLBUILDER_H
