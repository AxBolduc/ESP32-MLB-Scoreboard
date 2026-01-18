#include "NHLGame.h"

NHLGame::NHLGame(const JsonObject& game)
{
    // Parse NHL API JSON structure
    // Note: This is based on expected NHL API format - adjust when real API is integrated
    homeTeam = game["teams"]["home"]["abbr"].as<String>();
    homeTeamId = game["teams"]["home"]["id"].as<int>();
    awayTeam = game["teams"]["away"]["abbr"].as<String>();
    awayTeamId = game["teams"]["away"]["id"].as<int>();
    homeTeamScore = game["teams"]["home"]["score"].as<int>();
    awayTeamScore = game["teams"]["away"]["score"].as<int>();
    
    // Hockey-specific fields
    currentPeriod = game["period"]["current"].as<int>();
    timeRemaining = game["period"]["timeRemaining"].as<String>();
    
    // Determine overtime/shootout
    isShootout = game["period"]["isShootout"].as<bool>();
    isOvertime = game["period"]["isOvertime"].as<bool>();
}

NHLGame::~NHLGame()
{
    // Nothing to clean up
}
