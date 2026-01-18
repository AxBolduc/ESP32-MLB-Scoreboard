#ifndef INCLUDE_CORE_GAME_H
#define INCLUDE_CORE_GAME_H

#include <Arduino.h>

/**
 * @brief Abstract base class for game data across different sports
 * 
 * This interface defines the minimum common data that all sports must provide.
 * Sport-specific data (e.g., balls/strikes for baseball, period for hockey)
 * should be added in subclasses.
 */
class Game
{
public:
    virtual ~Game() = default;
    
    /**
     * @brief Get home team abbreviation
     * @return Team abbreviation string (e.g., "BOS", "TOR")
     */
    virtual String getHomeTeam() const = 0;
    
    /**
     * @brief Get home team ID
     * @return Team ID number
     */
    virtual int getHomeTeamId() const = 0;
    
    /**
     * @brief Get away team abbreviation
     * @return Team abbreviation string (e.g., "NYY", "MTL")
     */
    virtual String getAwayTeam() const = 0;
    
    /**
     * @brief Get away team ID
     * @return Team ID number
     */
    virtual int getAwayTeamId() const = 0;
    
    /**
     * @brief Get home team score
     * @return Current score for home team
     */
    virtual int getHomeTeamScore() const = 0;
    
    /**
     * @brief Get away team score
     * @return Current score for away team
     */
    virtual int getAwayTeamScore() const = 0;
};

#endif // INCLUDE_CORE_GAME_H
