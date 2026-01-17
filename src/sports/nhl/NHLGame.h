#ifndef INCLUDE_NHL_GAME_H
#define INCLUDE_NHL_GAME_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include "../../core/Game.h"

/**
 * @brief Represents a hockey game with current state
 * 
 * NHL-specific game data including period, time remaining, shots on goal,
 * and overtime/shootout indicators.
 */
class NHLGame : public Game
{
private:
    String homeTeam;
    int homeTeamId;
    String awayTeam;
    int awayTeamId;
    int homeTeamScore;
    int awayTeamScore;
    
    // Hockey-specific fields
    int currentPeriod;          // 1, 2, 3, 4 (OT), 5 (SO)
    String timeRemaining;       // "12:34" format
    int homeTeamShots;
    int awayTeamShots;
    bool isOvertime;            // True if in any overtime period
    bool isShootout;            // True if in shootout

public:
    explicit NHLGame(const JsonObject& game);
    ~NHLGame() override;
    
    // Implement base class interface
    String getHomeTeam() const override { return homeTeam; }
    int getHomeTeamId() const override { return homeTeamId; }
    String getAwayTeam() const override { return awayTeam; }
    int getAwayTeamId() const override { return awayTeamId; }
    int getHomeTeamScore() const override { return homeTeamScore; }
    int getAwayTeamScore() const override { return awayTeamScore; }
    
    // Hockey-specific getters
    int getCurrentPeriod() const { return currentPeriod; }
    String getTimeRemaining() const { return timeRemaining; }
    int getHomeTeamShots() const { return homeTeamShots; }
    int getAwayTeamShots() const { return awayTeamShots; }
    bool getIsOvertime() const { return isOvertime; }
    bool getIsShootout() const { return isShootout; }
};

#endif // INCLUDE_NHL_GAME_H
