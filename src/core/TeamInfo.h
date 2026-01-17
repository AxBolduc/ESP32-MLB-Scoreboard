#ifndef INCLUDE_CORE_TEAMINFO_H
#define INCLUDE_CORE_TEAMINFO_H

#include <Arduino.h>

/**
 * @brief Abstract base class for sport-specific team information
 * 
 * Each sport provides its own team data including IDs, colors, and navigation.
 * Teams should be stored in alphabetical order by city name.
 */
class TeamInfo
{
public:
    virtual ~TeamInfo() = default;
    
    /**
     * @brief Get total number of teams for this sport
     * @return Number of teams
     */
    virtual int getNumTeams() const = 0;
    
    /**
     * @brief Get team ID at a specific index
     * @param index Index in the team array (0 to getNumTeams()-1)
     * @return Team ID number
     */
    virtual int getTeamId(int index) const = 0;
    
    /**
     * @brief Get background color for a team
     * @param teamId The sport-specific team ID
     * @return RGB565 color value
     */
    virtual uint16_t getBackgroundColor(int teamId) const = 0;
    
    /**
     * @brief Get text color for a team
     * @param teamId The sport-specific team ID
     * @return RGB565 color value
     */
    virtual uint16_t getTextColor(int teamId) const = 0;
    
    /**
     * @brief Get the next team index (wraps around)
     * @param currentIndex Current index in team array
     * @return Next index (wraps to 0 at end)
     */
    virtual int getNextTeamIndex(int currentIndex) const = 0;
    
    /**
     * @brief Get the previous team index (wraps around)
     * @param currentIndex Current index in team array
     * @return Previous index (wraps to end at 0)
     */
    virtual int getPreviousTeamIndex(int currentIndex) const = 0;
    
    /**
     * @brief Get team abbreviation
     * @param teamId The sport-specific team ID
     * @return Team abbreviation (e.g., "BOS", "TOR")
     */
    virtual String getTeamAbbreviation(int teamId) const = 0;
};

#endif // INCLUDE_CORE_TEAMINFO_H
