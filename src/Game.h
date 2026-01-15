#ifndef INCLUDE_GAME_H
#define INCLUDE_GAME_H

#include <Arduino.h>
#include <ArduinoJson.h>

/**
 * @brief Represents a baseball game with current state
 */
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
    int balls;
    int strikes;
    int outs;
    bool topOfInning;
    bool firstBaseOccupied;
    bool secondBaseOccupied;
    bool thirdBaseOccupied;

public:
    explicit Game(const JsonObject& game);
    ~Game();
    
    // Getters - inline for performance
    String getHomeTeam() const { return homeTeam; }
    int getHomeTeamId() const { return homeTeamId; }
    String getAwayTeam() const { return awayTeam; }
    int getAwayTeamId() const { return awayTeamId; }
    int getHomeTeamScore() const { return homeTeamScore; }
    int getAwayTeamScore() const { return awayTeamScore; }
    int getBalls() const { return balls; }
    int getStrikes() const { return strikes; }
    int getOuts() const { return outs; }
    int getCurrentInning() const { return currentInning; }
    bool isTopOfInning() const { return topOfInning; }
    bool isFirstBaseOccupied() const { return firstBaseOccupied; }
    bool isSecondBaseOccupied() const { return secondBaseOccupied; }
    bool isThirdBaseOccupied() const { return thirdBaseOccupied; }
};

#endif // INCLUDE_GAME_H