#include "Game.h"

Game::Game(const JsonObject& game)
{
    homeTeam = game["teams"]["home"]["team"]["abbreviation"].as<String>();
    homeTeamId = game["teams"]["home"]["team"]["id"].as<int>();
    awayTeam = game["teams"]["away"]["team"]["abbreviation"].as<String>();
    awayTeamId = game["teams"]["away"]["team"]["id"].as<int>();
    homeTeamScore = game["linescore"]["teams"]["home"]["runs"].as<int>();
    awayTeamScore = game["linescore"]["teams"]["away"]["runs"].as<int>();
    balls = game["linescore"]["balls"].as<int>();
    strikes = game["linescore"]["strikes"].as<int>();
    outs = game["linescore"]["outs"].as<int>();
    currentInning = game["linescore"]["currentInning"].as<int>();
    topOfInning = game["linescore"]["isTopInning"].as<bool>();
    firstBaseOccupied = game["linescore"]["offense"]["first"].as<bool>();
    secondBaseOccupied = game["linescore"]["offense"]["second"].as<bool>();
    thirdBaseOccupied = game["linescore"]["offense"]["third"].as<bool>();
}

Game::~Game()
{
    // Nothing to clean up
}
