#ifndef INCLUDE_NHL_TEAMINFO_H
#define INCLUDE_NHL_TEAMINFO_H

#include <Arduino.h>
#include "../../core/TeamInfo.h"

// NHL Team IDs enum (alphabetically by city)
enum NHL_TEAM_ID
{
    ANAHEIM_DUCKS = 24,
    ARIZONA_COYOTES = 53,
    BOSTON_BRUINS = 6,
    BUFFALO_SABRES = 7,
    CALGARY_FLAMES = 20,
    CAROLINA_HURRICANES = 12,
    CHICAGO_BLACKHAWKS = 16,
    COLORADO_AVALANCHE = 21,
    COLUMBUS_BLUE_JACKETS = 29,
    DALLAS_STARS = 25,
    DETROIT_RED_WINGS = 17,
    EDMONTON_OILERS = 22,
    FLORIDA_PANTHERS = 13,
    LOS_ANGELES_KINGS = 26,
    MINNESOTA_WILD = 30,
    MONTREAL_CANADIENS = 8,
    NASHVILLE_PREDATORS = 18,
    NEW_JERSEY_DEVILS = 1,
    NEW_YORK_ISLANDERS = 2,
    NEW_YORK_RANGERS = 3,
    OTTAWA_SENATORS = 9,
    PHILADELPHIA_FLYERS = 4,
    PITTSBURGH_PENGUINS = 5,
    SAN_JOSE_SHARKS = 28,
    SEATTLE_KRAKEN = 55,
    ST_LOUIS_BLUES = 19,
    TAMPA_BAY_LIGHTNING = 14,
    TORONTO_MAPLE_LEAFS = 10,
    VANCOUVER_CANUCKS = 23,
    VEGAS_GOLDEN_KNIGHTS = 54,
    WASHINGTON_CAPITALS = 15,
    WINNIPEG_JETS = 52
};

// Color enum for NHL
enum NHLColor : uint16_t
{
    NHL_RED = 0x8000,
    NHL_YELLOW = 0xFFE0,
    NHL_WHITE = 0xFFFF,
    NHL_BLACK = 0x0000,
    NHL_NAVY_BLUE = 0x000F,
    NHL_ROYAL_BLUE = 0x435C,
    NHL_ORANGE = 0xFD20,
    NHL_GREEN = 0x07E0,
    NHL_GRAY = 0x8410,
    NHL_GOLD = 0xFEA0,
    NHL_TEAL = 0x4E99,
    NHL_MAROON = 0x7800,
    NHL_LIGHT_BLUE = 0x05BF
};

/**
 * @brief Manages NHL team information including IDs, colors, and navigation
 */
class NHLTeamInfo : public TeamInfo
{
public:
    static const int NUM_TEAMS = 32;
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

#endif // INCLUDE_NHL_TEAMINFO_H
