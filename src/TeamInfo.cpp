#include "TeamInfo.h"
#include <map>

// Team IDs array for cycling through teams
const int TeamInfo::TEAM_IDS[TeamInfo::NUM_TEAM_IDS] = {
    109, 110, 111, 112, 113, 114, 115, 116, 117, 118,
    119, 120, 121, 133, 134, 135, 136, 137, 138, 139,
    140, 141, 142, 143, 144, 145, 146, 147, 158
};

// Team background colors map
static const std::map<int, uint16_t> TEAM_BG_COLORS = {
    {TEAM_ID::ATLANTA_BRAVES, Color::NAVY_BLUE},
    {TEAM_ID::ARIZONA_DIAMONDBACKS, Color::RED},
    {TEAM_ID::BALTIMORE_ORIOLES, Color::ORANGE},
    {TEAM_ID::BOSTON_REDSOX, Color::RED},
    {TEAM_ID::CHICAGO_CUBS, Color::ROYAL_BLUE},
    {TEAM_ID::CHICAGO_WHITE_SOX, Color::BLACK},
    {TEAM_ID::CINCINNATI_REDS, Color::RED},
    {TEAM_ID::CLEVELAND_INDIANS, Color::NAVY_BLUE},
    {TEAM_ID::COLORADO_ROCKIES, Color::BLACK},
    {TEAM_ID::DETROIT_TIGERS, Color::BLACK},
    {TEAM_ID::HOUSTON_ASTROS, Color::ORANGE},
    {TEAM_ID::KANSAS_CITY_ROYALS, Color::ROYAL_BLUE},
    {TEAM_ID::LOS_ANGELES_ANGELS, Color::RED},
    {TEAM_ID::LOS_ANGELES_DODGERS, Color::ROYAL_BLUE},
    {TEAM_ID::MIAMI_MARLINS, Color::ROYAL_BLUE},
    {TEAM_ID::MILWAUKEE_BREWERS, Color::NAVY_BLUE},
    {TEAM_ID::MINNESOTA_TWINS, Color::NAVY_BLUE},
    {TEAM_ID::NEW_YORK_METS, Color::ROYAL_BLUE},
    {TEAM_ID::NEW_YORK_YANKEES, Color::BLACK},
    {TEAM_ID::OAKLAND_ATHLETICS, Color::GREEN},
    {TEAM_ID::PHILADELPHIA_PHILLIES, Color::RED},
    {TEAM_ID::PITTSBURGH_PIRATES, Color::BLACK},
    {TEAM_ID::SAN_DIEGO_PADRES, Color::BROWN},
    {TEAM_ID::SAN_FRANCISCO_GIANTS, Color::BLACK},
    {TEAM_ID::SEATTLE_MARINERS, Color::NAVY_BLUE},
    {TEAM_ID::ST_LOUIS_CARDINALS, Color::RED},
    {TEAM_ID::TAMPA_BAY_RAYS, Color::NAVY_BLUE},
    {TEAM_ID::TEXAS_RANGERS, Color::ROYAL_BLUE},
    {TEAM_ID::TORONTO_BLUE_JAYS, Color::ROYAL_BLUE},
    {TEAM_ID::WASHINGTON_NATIONALS, Color::RED}
};

// Team text colors map
static const std::map<int, uint16_t> TEAMS_TEXT_COLORS = {
    {TEAM_ID::ATLANTA_BRAVES, Color::RED},
    {TEAM_ID::ARIZONA_DIAMONDBACKS, Color::BLACK},
    {TEAM_ID::BALTIMORE_ORIOLES, Color::BLACK},
    {TEAM_ID::BOSTON_REDSOX, Color::WHITE},
    {TEAM_ID::CHICAGO_CUBS, Color::RED},
    {TEAM_ID::CHICAGO_WHITE_SOX, Color::WHITE},
    {TEAM_ID::CINCINNATI_REDS, Color::WHITE},
    {TEAM_ID::CLEVELAND_INDIANS, Color::RED},
    {TEAM_ID::COLORADO_ROCKIES, Color::PURPLE},
    {TEAM_ID::DETROIT_TIGERS, Color::WHITE},
    {TEAM_ID::HOUSTON_ASTROS, Color::NAVY_BLUE},
    {TEAM_ID::KANSAS_CITY_ROYALS, Color::WHITE},
    {TEAM_ID::LOS_ANGELES_ANGELS, Color::WHITE},
    {TEAM_ID::LOS_ANGELES_DODGERS, Color::WHITE},
    {TEAM_ID::MIAMI_MARLINS, Color::WHITE},
    {TEAM_ID::MILWAUKEE_BREWERS, Color::YELLOW},
    {TEAM_ID::MINNESOTA_TWINS, Color::WHITE},
    {TEAM_ID::NEW_YORK_METS, Color::ORANGE},
    {TEAM_ID::NEW_YORK_YANKEES, Color::WHITE},
    {TEAM_ID::OAKLAND_ATHLETICS, Color::BLACK},
    {TEAM_ID::PHILADELPHIA_PHILLIES, Color::WHITE},
    {TEAM_ID::PITTSBURGH_PIRATES, Color::YELLOW},
    {TEAM_ID::SAN_DIEGO_PADRES, Color::YELLOW},
    {TEAM_ID::SAN_FRANCISCO_GIANTS, Color::ORANGE},
    {TEAM_ID::SEATTLE_MARINERS, Color::WHITE},
    {TEAM_ID::ST_LOUIS_CARDINALS, Color::WHITE},
    {TEAM_ID::TAMPA_BAY_RAYS, Color::ROYAL_BLUE},
    {TEAM_ID::TEXAS_RANGERS, Color::RED},
    {TEAM_ID::TORONTO_BLUE_JAYS, Color::WHITE},
    {TEAM_ID::WASHINGTON_NATIONALS, Color::WHITE}
};

uint16_t TeamInfo::getBackgroundColor(int teamId)
{
    auto it = TEAM_BG_COLORS.find(teamId);
    if (it != TEAM_BG_COLORS.end())
    {
        return it->second;
    }
    return Color::BLACK;  // Default color
}

uint16_t TeamInfo::getTextColor(int teamId)
{
    auto it = TEAMS_TEXT_COLORS.find(teamId);
    if (it != TEAMS_TEXT_COLORS.end())
    {
        return it->second;
    }
    return Color::WHITE;  // Default color
}

TEAM_ID TeamInfo::getNextTeam(TEAM_ID current)
{
    int currentIdx = 0;
    for (int i = 0; i < NUM_TEAM_IDS; i++)
    {
        if (current == TEAM_IDS[i])
        {
            currentIdx = i;
            break;
        }
    }
    return static_cast<TEAM_ID>(TEAM_IDS[(currentIdx + 1) % NUM_TEAM_IDS]);
}

TEAM_ID TeamInfo::getPreviousTeam(TEAM_ID current)
{
    int currentIdx = 0;
    for (int i = 0; i < NUM_TEAM_IDS; i++)
    {
        if (current == TEAM_IDS[i])
        {
            currentIdx = i;
            break;
        }
    }
    return static_cast<TEAM_ID>(TEAM_IDS[(currentIdx - 1 + NUM_TEAM_IDS) % NUM_TEAM_IDS]);
}

// Operator overloads for backward compatibility
TEAM_ID operator++(TEAM_ID &id)
{
    id = TeamInfo::getNextTeam(id);
    return id;
}

TEAM_ID operator++(TEAM_ID &id, int)  // postfix
{
    TEAM_ID result = id;
    ++id;
    return result;
}
