#include "MLBGame.h"

MLBGame::MLBGame(const JsonObject& game)
{
    homeTeam = game["teams"]["home"]["abbr"].as<String>();
    homeTeamId = game["teams"]["home"]["id"].as<int>();
    awayTeam = game["teams"]["away"]["abbr"].as<String>();
    awayTeamId = game["teams"]["away"]["id"].as<int>();
    homeTeamScore = game["teams"]["home"]["score"].as<int>();
    awayTeamScore = game["teams"]["away"]["score"].as<int>();
    balls = game["meta"]["count"]["balls"].as<int>();
    strikes = game["meta"]["count"]["strikes"].as<int>();
    outs = game["meta"]["count"]["outs"].as<int>();
    currentInning = game["period"]["current"].as<int>();
    topOfInning = game["period"]["isTop"].as<bool>();
    firstBaseOccupied = game["meta"]["bases"]["first"].as<bool>();
    secondBaseOccupied = game["meta"]["bases"]["second"].as<bool>();
    thirdBaseOccupied = game["meta"]["bases"]["third"].as<bool>();
}

MLBGame::~MLBGame()
{
    // Nothing to clean up
}
