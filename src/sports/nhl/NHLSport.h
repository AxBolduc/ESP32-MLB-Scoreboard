#ifndef INCLUDE_NHL_SPORT_H
#define INCLUDE_NHL_SPORT_H

#include "../../core/Sport.h"
#include "NHLGame.h"
#include "NHLGameDrawer.h"
#include "NHLApiHandler.h"
#include "NHLTeamInfo.h"

/**
 * @brief NHL Sport implementation
 * 
 * Orchestrates all NHL-specific components (game, drawer, API, teams).
 */
class NHLSport : public Sport
{
private:
    NHLTeamInfo teamInfo;  // Owned by NHLSport
    
public:
    /**
     * @brief Construct a new NHL Sport object
     * @param display Pointer to LED matrix display (not owned)
     */
    explicit NHLSport(MatrixPanel_I2S_DMA* display);
    
    /**
     * @brief Destroy the NHL Sport object
     */
    ~NHLSport() override;
    
    // Factory methods - create NHL-specific instances
    GameDrawer* createDrawer() override;
    ApiHandler* createApiHandler() override;
    TeamInfo* getTeamInfo() override;
    Game* createGame(const JsonObject& data) override;
    
    // Sport identification
    String getSportName() const override;
    SportType getSportType() const override;
};

#endif // INCLUDE_NHL_SPORT_H
