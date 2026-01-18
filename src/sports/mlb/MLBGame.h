#ifndef INCLUDE_MLB_GAME_H
#define INCLUDE_MLB_GAME_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include "../../core/Game.h"

/**
 * @brief Represents a baseball game with current state
 * 
 * MLB-specific game data including baseball elements like innings,
 * balls, strikes, outs, and base runners.
 */
class MLBGame : public Game
{
private:
    String homeTeam;
    int homeTeamId;
    String awayTeam;
    int awayTeamId;
    int homeTeamScore;
    int awayTeamScore;
    
    // Baseball-specific fields
    int currentInning;
    int balls;
    int strikes;
    int outs;
    bool topOfInning;
    bool firstBaseOccupied;
    bool secondBaseOccupied;
    bool thirdBaseOccupied;

public:
    explicit MLBGame(const JsonObject& game);
    ~MLBGame() override;
    
    // Implement base class interface
    String getHomeTeam() const override { return homeTeam; }
    int getHomeTeamId() const override { return homeTeamId; }
    String getAwayTeam() const override { return awayTeam; }
    int getAwayTeamId() const override { return awayTeamId; }
    int getHomeTeamScore() const override { return homeTeamScore; }
    int getAwayTeamScore() const override { return awayTeamScore; }
    
    // Baseball-specific getters
    int getBalls() const { return balls; }
    int getStrikes() const { return strikes; }
    int getOuts() const { return outs; }
    int getCurrentInning() const { return currentInning; }
    bool isTopOfInning() const { return topOfInning; }
    bool isFirstBaseOccupied() const { return firstBaseOccupied; }
    bool isSecondBaseOccupied() const { return secondBaseOccupied; }
    bool isThirdBaseOccupied() const { return thirdBaseOccupied; }
};

#endif // INCLUDE_MLB_GAME_H
