#include "NHLGame.h"

NHLGame::NHLGame(const JsonObject& game)
{
    // Parse NHL API JSON structure
    // Note: This is based on expected NHL API format - adjust when real API is integrated
    homeTeam = game["teams"]["home"]["team"]["abbreviation"].as<String>();
    homeTeamId = game["teams"]["home"]["team"]["id"].as<int>();
    awayTeam = game["teams"]["away"]["team"]["abbreviation"].as<String>();
    awayTeamId = game["teams"]["away"]["team"]["id"].as<int>();
    homeTeamScore = game["teams"]["home"]["score"].as<int>();
    awayTeamScore = game["teams"]["away"]["score"].as<int>();
    
    // Hockey-specific fields
    currentPeriod = game["linescore"]["currentPeriod"].as<int>();
    timeRemaining = game["linescore"]["currentPeriodTimeRemaining"].as<String>();
    
    homeTeamShots = game["teams"]["home"]["shots"].as<int>();
    awayTeamShots = game["teams"]["away"]["shots"].as<int>();
    
    // Determine overtime/shootout
    isShootout = (currentPeriod == 5);  // Period 5 = shootout
    isOvertime = (currentPeriod > 3 && !isShootout);  // Period 4+ (excluding shootout)
}

NHLGame::~NHLGame()
{
    // Nothing to clean up
}
