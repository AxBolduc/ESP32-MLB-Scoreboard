#ifndef INCLUDE_MLB_SPORT_H
#define INCLUDE_MLB_SPORT_H

#include "../../core/Sport.h"
#include "MLBGame.h"
#include "MLBGameDrawer.h"
#include "MLBApiHandler.h"
#include "MLBTeamInfo.h"

/**
 * @brief MLB Sport implementation
 * 
 * Orchestrates all MLB-specific components (game, drawer, API, teams).
 */
class MLBSport : public Sport
{
private:
    MLBTeamInfo teamInfo;  // Owned by MLBSport
    
public:
    /**
     * @brief Construct a new MLB Sport object
     * @param display Pointer to LED matrix display (not owned)
     */
    explicit MLBSport(MatrixPanel_I2S_DMA* display);
    
    /**
     * @brief Destroy the MLB Sport object
     */
    ~MLBSport() override;
    
    // Factory methods - create MLB-specific instances
    GameDrawer* createDrawer() override;
    ApiHandler* createApiHandler() override;
    TeamInfo* getTeamInfo() override;
    Game* createGame(const JsonObject& data) override;
    
    // Sport identification
    String getSportName() const override;
    SportType getSportType() const override;
};

#endif // INCLUDE_MLB_SPORT_H
