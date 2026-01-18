#ifndef INCLUDE_MLB_GAMEDRAWER_H
#define INCLUDE_MLB_GAMEDRAWER_H

#include <Arduino.h>
#include <ESP32-HUB75-MatrixPanel-I2S-DMA.h>
#include "../../core/GameDrawer.h"
#include "../../core/Game.h"
#include "MLBGame.h"

/**
 * @brief Handles rendering MLB baseball game information to the LED matrix
 * 
 * Inherits shared drawing utilities from core/GameDrawer and implements
 * baseball-specific rendering (bases, innings, balls/strikes/outs).
 */
class MLBGameDrawer : public GameDrawer
{
private:
    void drawLayout(const MLBGame* game);
    void drawScores(const MLBGame* game);
    void drawCount(const MLBGame* game);
    void drawInning(const MLBGame* game);
    void topInningTriangle();
    void bottomInningTriangle();
    void drawBases(const MLBGame* game);
    void drawBase(int16_t x, int16_t y, bool occupied);

public:
    /**
     * @brief Construct a new MLB Game Drawer
     * @param display Pointer to the display matrix (not owned)
     */
    explicit MLBGameDrawer(MatrixPanel_I2S_DMA* display);
    ~MLBGameDrawer() override;
    
    /**
     * @brief Draw the current MLB game state
     * @param game Pointer to game data (base class interface)
     */
    void drawGame(const Game* game) override;
};

#endif // INCLUDE_MLB_GAMEDRAWER_H
