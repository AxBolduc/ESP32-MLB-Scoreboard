#ifndef INCLUDE_GAME_H
#define INCLUDE_GAME_H

#include "ArduinoJson.h"

class Game
{
private:
    String homeTeam;
    int homeTeamId;
    String awayTeam;
    int awayTeamId;
    int homeTeamScore;
    int awayTeamScore;
    int currentInning;
    bool topOfInning;
    bool firstBaseOccupied;
    bool secondBaseOccupied;
    bool thirdBaseOccupied;

    /* data */
public:
    Game(JsonObject game);
    ~Game();
    String getHomeTeam() { return this->homeTeam; };
    int getHomeTeamId() { return this->homeTeamId; };
    String getAwayTeam() { return this->awayTeam; };
    int getAwayTeamId() { return this->awayTeamId; };
    int getHomeTeamScore() { return this->homeTeamScore; };
    int getAwayTeamScore() { return this->awayTeamScore; };
    int getCurrentInning() { return this->currentInning; };
    bool isTopOfInning() { return this->topOfInning; };
    bool isFirstBaseOccupied() { return this->firstBaseOccupied; };
    bool isSecondBaseOccupied() { return this->secondBaseOccupied; };
    bool isThirdBaseOccupied() { return this->thirdBaseOccupied; };
};

Game::Game(JsonObject game)
{
    this->homeTeam = game["teams"]["home"]["team"]["abbreviation"].as<String>();
    this->homeTeamId = game["teams"]["home"]["team"]["id"].as<int>();
    this->awayTeam = game["teams"]["away"]["team"]["abbreviation"].as<String>();
    this->awayTeamId = game["teams"]["away"]["team"]["id"].as<int>();
    this->homeTeamScore = game["linescore"]["teams"]["home"]["runs"].as<int>();
    this->awayTeamScore = game["linescore"]["teams"]["away"]["runs"].as<int>();
    this->currentInning = game["linescore"]["currentInning"].as<int>();
    this->topOfInning = game["linescore"]["isTopInning"].as<bool>();
    this->firstBaseOccupied = game["linescore"]["offense"]["first"].as<bool>();
    this->secondBaseOccupied = game["linescore"]["offense"]["second"].as<bool>();
    this->thirdBaseOccupied = game["linescore"]["offense"]["third"].as<bool>();

    Serial.println(game["linescore"]["currentInning"].as<int>());
}

Game::~Game()
{
}

#endif // INCLUDE_GAME_H