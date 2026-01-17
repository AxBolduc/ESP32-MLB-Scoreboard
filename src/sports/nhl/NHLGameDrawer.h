#ifndef INCLUDE_NHL_GAMEDRAWER_H
#define INCLUDE_NHL_GAMEDRAWER_H

#include <Arduino.h>
#include <ESP32-HUB75-MatrixPanel-I2S-DMA.h>
#include "../../core/GameDrawer.h"
#include "../../core/Game.h"
#include "NHLGame.h"

/**
 * @brief Handles rendering NHL hockey game information to the LED matrix
 * 
 * Inherits shared drawing utilities from core/GameDrawer and implements
 * hockey-specific rendering (period, time, shots on goal, OT/SO indicators).
 */
class NHLGameDrawer : public GameDrawer
{
private:
    void drawLayout(const NHLGame* game);
    void drawScores(const NHLGame* game);
    void drawPeriod(const NHLGame* game);
    void drawTime(const NHLGame* game);
    void drawShots(const NHLGame* game);
    void drawOvertimeIndicator(const NHLGame* game);

public:
    /**
     * @brief Construct a new NHL Game Drawer
     * @param display Pointer to the display matrix (not owned)
     */
    explicit NHLGameDrawer(MatrixPanel_I2S_DMA* display);
    ~NHLGameDrawer() override;
    
    /**
     * @brief Draw the current NHL game state
     * @param game Pointer to game data (base class interface)
     */
    void drawGame(const Game* game) override;
};

#endif // INCLUDE_NHL_GAMEDRAWER_H
