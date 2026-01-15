#ifndef INCLUDE_TEAMINFO_H
#define INCLUDE_TEAMINFO_H

#include <Arduino.h>

enum TEAM_ID
{
    ATLANTA_BRAVES = 144,
    ARIZONA_DIAMONDBACKS = 109,
    BALTIMORE_ORIOLES = 110,
    BOSTON_REDSOX = 111,
    CHICAGO_CUBS = 112,
    CHICAGO_WHITE_SOX = 145,
    CINCINNATI_REDS = 113,
    CLEVELAND_INDIANS = 114,
    COLORADO_ROCKIES = 115,
    DETROIT_TIGERS = 116,
    HOUSTON_ASTROS = 117,
    KANSAS_CITY_ROYALS = 118,
    LOS_ANGELES_ANGELS = 108,
    LOS_ANGELES_DODGERS = 119,
    MIAMI_MARLINS = 146,
    MILWAUKEE_BREWERS = 158,
    MINNESOTA_TWINS = 142,
    NEW_YORK_METS = 121,
    NEW_YORK_YANKEES = 147,
    OAKLAND_ATHLETICS = 133,
    PHILADELPHIA_PHILLIES = 143,
    PITTSBURGH_PIRATES = 134,
    SAN_DIEGO_PADRES = 135,
    SAN_FRANCISCO_GIANTS = 137,
    SEATTLE_MARINERS = 136,
    ST_LOUIS_CARDINALS = 138,
    TAMPA_BAY_RAYS = 139,
    TEXAS_RANGERS = 140,
    TORONTO_BLUE_JAYS = 141,
    WASHINGTON_NATIONALS = 120
};

enum Color : uint16_t
{
    RED = 0x8000,
    YELLOW = 0xFFE0,
    WHITE = 0xFFFF,
    BLACK = 0x0000,
    NAVY_BLUE = 0x000F,
    ROYAL_BLUE = 0x435C,
    ORANGE = 0xFD20,
    GREEN = 0x07E0,
    GRAY = 0x8410,
    BROWN = 0xA145,
    PURPLE = 0x8010
};

/**
 * @brief Manages team information including IDs, colors, and navigation
 */
class TeamInfo
{
public:
    static const int NUM_TEAM_IDS = 30;
    static const int TEAM_IDS[NUM_TEAM_IDS];
    
    /**
     * @brief Get the background color for a team
     * @param teamId The MLB team ID
     * @return RGB565 color value
     */
    static uint16_t getBackgroundColor(int teamId);
    
    /**
     * @brief Get the text color for a team
     * @param teamId The MLB team ID
     * @return RGB565 color value
     */
    static uint16_t getTextColor(int teamId);
    
    /**
     * @brief Get the next team in the rotation
     * @param current Current team ID
     * @return Next team ID (wraps around)
     */
    static TEAM_ID getNextTeam(TEAM_ID current);
    
    /**
     * @brief Get the previous team in the rotation
     * @param current Current team ID
     * @return Previous team ID (wraps around)
     */
    static TEAM_ID getPreviousTeam(TEAM_ID current);

private:
    TeamInfo() = delete;  // No instantiation
    TeamInfo(const TeamInfo&) = delete;
    TeamInfo& operator=(const TeamInfo&) = delete;
};

// Operator overloads for backward compatibility
TEAM_ID operator++(TEAM_ID &id);
TEAM_ID operator++(TEAM_ID &id, int);

#endif // INCLUDE_TEAMINFO_H
