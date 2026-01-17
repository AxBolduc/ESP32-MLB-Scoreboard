#include "MLBTeamInfo.h"
#include <map>

// Team IDs array for cycling through teams (alphabetical by city)
const int MLBTeamInfo::TEAM_IDS[MLBTeamInfo::NUM_TEAMS] = {
    109, 144, 110, 111, 145, 112, 113, 114, 115, 116,
    117, 118, 108, 119, 146, 158, 142, 121, 147, 133,
    143, 134, 135, 137, 136, 138, 139, 140, 141, 120
};

// Team background colors map
static const std::map<int, uint16_t> TEAM_BG_COLORS = {
    {MLB_TEAM_ID::ATLANTA_BRAVES, MLBColor::MLB_NAVY_BLUE},
    {MLB_TEAM_ID::ARIZONA_DIAMONDBACKS, MLBColor::MLB_RED},
    {MLB_TEAM_ID::BALTIMORE_ORIOLES, MLBColor::MLB_ORANGE},
    {MLB_TEAM_ID::BOSTON_REDSOX, MLBColor::MLB_RED},
    {MLB_TEAM_ID::CHICAGO_CUBS, MLBColor::MLB_ROYAL_BLUE},
    {MLB_TEAM_ID::CHICAGO_WHITE_SOX, MLBColor::MLB_BLACK},
    {MLB_TEAM_ID::CINCINNATI_REDS, MLBColor::MLB_RED},
    {MLB_TEAM_ID::CLEVELAND_INDIANS, MLBColor::MLB_NAVY_BLUE},
    {MLB_TEAM_ID::COLORADO_ROCKIES, MLBColor::MLB_BLACK},
    {MLB_TEAM_ID::DETROIT_TIGERS, MLBColor::MLB_BLACK},
    {MLB_TEAM_ID::HOUSTON_ASTROS, MLBColor::MLB_ORANGE},
    {MLB_TEAM_ID::KANSAS_CITY_ROYALS, MLBColor::MLB_ROYAL_BLUE},
    {MLB_TEAM_ID::LOS_ANGELES_ANGELS, MLBColor::MLB_RED},
    {MLB_TEAM_ID::LOS_ANGELES_DODGERS, MLBColor::MLB_ROYAL_BLUE},
    {MLB_TEAM_ID::MIAMI_MARLINS, MLBColor::MLB_ROYAL_BLUE},
    {MLB_TEAM_ID::MILWAUKEE_BREWERS, MLBColor::MLB_NAVY_BLUE},
    {MLB_TEAM_ID::MINNESOTA_TWINS, MLBColor::MLB_NAVY_BLUE},
    {MLB_TEAM_ID::NEW_YORK_METS, MLBColor::MLB_ROYAL_BLUE},
    {MLB_TEAM_ID::NEW_YORK_YANKEES, MLBColor::MLB_BLACK},
    {MLB_TEAM_ID::OAKLAND_ATHLETICS, MLBColor::MLB_GREEN},
    {MLB_TEAM_ID::PHILADELPHIA_PHILLIES, MLBColor::MLB_RED},
    {MLB_TEAM_ID::PITTSBURGH_PIRATES, MLBColor::MLB_BLACK},
    {MLB_TEAM_ID::SAN_DIEGO_PADRES, MLBColor::MLB_BROWN},
    {MLB_TEAM_ID::SAN_FRANCISCO_GIANTS, MLBColor::MLB_BLACK},
    {MLB_TEAM_ID::SEATTLE_MARINERS, MLBColor::MLB_NAVY_BLUE},
    {MLB_TEAM_ID::ST_LOUIS_CARDINALS, MLBColor::MLB_RED},
    {MLB_TEAM_ID::TAMPA_BAY_RAYS, MLBColor::MLB_NAVY_BLUE},
    {MLB_TEAM_ID::TEXAS_RANGERS, MLBColor::MLB_ROYAL_BLUE},
    {MLB_TEAM_ID::TORONTO_BLUE_JAYS, MLBColor::MLB_ROYAL_BLUE},
    {MLB_TEAM_ID::WASHINGTON_NATIONALS, MLBColor::MLB_RED}
};

// Team text colors map
static const std::map<int, uint16_t> TEAMS_TEXT_COLORS = {
    {MLB_TEAM_ID::ATLANTA_BRAVES, MLBColor::MLB_RED},
    {MLB_TEAM_ID::ARIZONA_DIAMONDBACKS, MLBColor::MLB_BLACK},
    {MLB_TEAM_ID::BALTIMORE_ORIOLES, MLBColor::MLB_BLACK},
    {MLB_TEAM_ID::BOSTON_REDSOX, MLBColor::MLB_WHITE},
    {MLB_TEAM_ID::CHICAGO_CUBS, MLBColor::MLB_RED},
    {MLB_TEAM_ID::CHICAGO_WHITE_SOX, MLBColor::MLB_WHITE},
    {MLB_TEAM_ID::CINCINNATI_REDS, MLBColor::MLB_WHITE},
    {MLB_TEAM_ID::CLEVELAND_INDIANS, MLBColor::MLB_RED},
    {MLB_TEAM_ID::COLORADO_ROCKIES, MLBColor::MLB_PURPLE},
    {MLB_TEAM_ID::DETROIT_TIGERS, MLBColor::MLB_WHITE},
    {MLB_TEAM_ID::HOUSTON_ASTROS, MLBColor::MLB_NAVY_BLUE},
    {MLB_TEAM_ID::KANSAS_CITY_ROYALS, MLBColor::MLB_WHITE},
    {MLB_TEAM_ID::LOS_ANGELES_ANGELS, MLBColor::MLB_WHITE},
    {MLB_TEAM_ID::LOS_ANGELES_DODGERS, MLBColor::MLB_WHITE},
    {MLB_TEAM_ID::MIAMI_MARLINS, MLBColor::MLB_WHITE},
    {MLB_TEAM_ID::MILWAUKEE_BREWERS, MLBColor::MLB_YELLOW},
    {MLB_TEAM_ID::MINNESOTA_TWINS, MLBColor::MLB_WHITE},
    {MLB_TEAM_ID::NEW_YORK_METS, MLBColor::MLB_ORANGE},
    {MLB_TEAM_ID::NEW_YORK_YANKEES, MLBColor::MLB_WHITE},
    {MLB_TEAM_ID::OAKLAND_ATHLETICS, MLBColor::MLB_BLACK},
    {MLB_TEAM_ID::PHILADELPHIA_PHILLIES, MLBColor::MLB_WHITE},
    {MLB_TEAM_ID::PITTSBURGH_PIRATES, MLBColor::MLB_YELLOW},
    {MLB_TEAM_ID::SAN_DIEGO_PADRES, MLBColor::MLB_YELLOW},
    {MLB_TEAM_ID::SAN_FRANCISCO_GIANTS, MLBColor::MLB_ORANGE},
    {MLB_TEAM_ID::SEATTLE_MARINERS, MLBColor::MLB_WHITE},
    {MLB_TEAM_ID::ST_LOUIS_CARDINALS, MLBColor::MLB_WHITE},
    {MLB_TEAM_ID::TAMPA_BAY_RAYS, MLBColor::MLB_ROYAL_BLUE},
    {MLB_TEAM_ID::TEXAS_RANGERS, MLBColor::MLB_RED},
    {MLB_TEAM_ID::TORONTO_BLUE_JAYS, MLBColor::MLB_WHITE},
    {MLB_TEAM_ID::WASHINGTON_NATIONALS, MLBColor::MLB_WHITE}
};

int MLBTeamInfo::getTeamId(int index) const
{
    if (index >= 0 && index < NUM_TEAMS)
    {
        return TEAM_IDS[index];
    }
    return TEAM_IDS[0];  // Default to first team
}

uint16_t MLBTeamInfo::getBackgroundColor(int teamId) const
{
    auto it = TEAM_BG_COLORS.find(teamId);
    if (it != TEAM_BG_COLORS.end())
    {
        return it->second;
    }
    return MLBColor::MLB_BLACK;  // Default color
}

uint16_t MLBTeamInfo::getTextColor(int teamId) const
{
    auto it = TEAMS_TEXT_COLORS.find(teamId);
    if (it != TEAMS_TEXT_COLORS.end())
    {
        return it->second;
    }
    return MLBColor::MLB_WHITE;  // Default color
}

int MLBTeamInfo::getNextTeamIndex(int currentIndex) const
{
    return (currentIndex + 1) % NUM_TEAMS;
}

int MLBTeamInfo::getPreviousTeamIndex(int currentIndex) const
{
    return (currentIndex - 1 + NUM_TEAMS) % NUM_TEAMS;
}

String MLBTeamInfo::getTeamAbbreviation(int teamId) const
{
    // This would need a map for full implementation
    // For now, return empty (API provides abbreviations)
    return "";
}
