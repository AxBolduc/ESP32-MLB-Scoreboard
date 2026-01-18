#ifndef INCLUDE_MLB_TEAMINFO_H
#define INCLUDE_MLB_TEAMINFO_H

#include <Arduino.h>
#include "../../core/TeamInfo.h"

// MLB Team IDs enum
enum MLB_TEAM_ID
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

// Color enum for MLB
enum MLBColor : uint16_t
{
    MLB_RED = 0x8000,
    MLB_YELLOW = 0xFFE0,
    MLB_WHITE = 0xFFFF,
    MLB_BLACK = 0x0000,
    MLB_NAVY_BLUE = 0x000F,
    MLB_ROYAL_BLUE = 0x435C,
    MLB_ORANGE = 0xFD20,
    MLB_GREEN = 0x07E0,
    MLB_GRAY = 0x8410,
    MLB_BROWN = 0xA145,
    MLB_PURPLE = 0x8010
};

/**
 * @brief Manages MLB team information including IDs, colors, and navigation
 */
class MLBTeamInfo : public TeamInfo
{
public:
    static const int NUM_TEAMS = 30;
    static const int TEAM_IDS[NUM_TEAMS];
    
    // Implement base class interface
    int getNumTeams() const override { return NUM_TEAMS; }
    int getTeamId(int index) const override;
    uint16_t getBackgroundColor(int teamId) const override;
    uint16_t getTextColor(int teamId) const override;
    int getNextTeamIndex(int currentIndex) const override;
    int getPreviousTeamIndex(int currentIndex) const override;
    String getTeamAbbreviation(int teamId) const override;
};

#endif // INCLUDE_MLB_TEAMINFO_H
