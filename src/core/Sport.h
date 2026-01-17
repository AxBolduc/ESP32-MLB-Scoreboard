#ifndef INCLUDE_CORE_SPORT_H
#define INCLUDE_CORE_SPORT_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESP32-HUB75-MatrixPanel-I2S-DMA.h>
#include "Game.h"
#include "GameDrawer.h"
#include "ApiHandler.h"
#include "TeamInfo.h"

/**
 * @brief Sport type enumeration
 */
enum class SportType
{
    MLB,    // Major League Baseball
    NHL     // National Hockey League
    // Future: NBA, NFL, MLS, etc.
};

/**
 * @brief Abstract base class for sport implementations
 * 
 * Each sport (MLB, NHL, etc.) inherits from this class and provides
 * sport-specific implementations for games, drawing, API handling, and teams.
 * 
 * Ownership model:
 * - Sport owns: drawer, apiHandler, currentGame
 * - Sport does NOT own: display (injected)
 */
class Sport
{
protected:
    MatrixPanel_I2S_DMA* display;  // Injected, not owned
    GameDrawer* drawer;            // Owned by Sport
    ApiHandler* apiHandler;        // Owned by Sport
    Game* currentGame;             // Owned by Sport
    int currentTeamIndex;          // Index into sport's team array
    
public:
    /**
     * @brief Construct a new Sport object
     * @param display Pointer to LED matrix display (not owned)
     */
    explicit Sport(MatrixPanel_I2S_DMA* display);
    
    /**
     * @brief Virtual destructor - cleans up owned resources
     */
    virtual ~Sport();
    
    /**
     * @brief Create sport-specific drawer (factory method)
     * @return Pointer to newly created GameDrawer (caller must manage lifetime)
     */
    virtual GameDrawer* createDrawer() = 0;
    
    /**
     * @brief Create sport-specific API handler (factory method)
     * @return Pointer to newly created ApiHandler (caller must manage lifetime)
     */
    virtual ApiHandler* createApiHandler() = 0;
    
    /**
     * @brief Get sport-specific team information
     * @return Pointer to TeamInfo object (owned by Sport subclass)
     */
    virtual TeamInfo* getTeamInfo() = 0;
    
    /**
     * @brief Create a game object from JSON data (factory method)
     * @param data JSON object containing game data
     * @return Pointer to newly created Game (caller must manage lifetime)
     */
    virtual Game* createGame(const JsonObject& data) = 0;
    
    /**
     * @brief Get sport name
     * @return Sport name string (e.g., "MLB", "NHL")
     */
    virtual String getSportName() const = 0;
    
    /**
     * @brief Get sport type enum
     * @return SportType enum value
     */
    virtual SportType getSportType() const = 0;
    
    /**
     * @brief Update display with current team's game data
     * 
     * Default implementation:
     * 1. Fetch schedule from API
     * 2. Parse game data
     * 3. Create Game object
     * 4. Draw to display
     * 
     * Can be overridden for sport-specific behavior.
     */
    virtual void update();
    
    /**
     * @brief Switch to next team in rotation
     */
    virtual void nextTeam();
    
    /**
     * @brief Switch to previous team in rotation
     */
    virtual void previousTeam();
    
    /**
     * @brief Get current game object
     * @return Pointer to current game (may be null)
     */
    Game* getCurrentGame() const { return currentGame; }
    
    /**
     * @brief Get current team ID
     * @return Team ID from current team index
     */
    int getCurrentTeamId() const;
    
    /**
     * @brief Get game drawer
     * @return Pointer to game drawer
     */
    GameDrawer* getDrawer() const { return drawer; }
};

#endif // INCLUDE_CORE_SPORT_H
