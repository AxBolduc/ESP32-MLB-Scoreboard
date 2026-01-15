#ifndef INCLUDE_GAMEDRAWER_H
#define INCLUDE_GAMEDRAWER_H

#include <Arduino.h>
#include <ESP32-HUB75-MatrixPanel-I2S-DMA.h>
#include "Game.h"

/**
 * @brief Handles rendering game information to the LED matrix
 */
class GameDrawer
{
private:
    MatrixPanel_I2S_DMA* display;
    
    void drawLayout(const Game* game);
    void drawScores(const Game* game);
    void drawCount(const Game* game);
    void drawBox(int16_t x, int16_t y, const String& text, uint16_t bg_color, uint16_t fg_color);
    void drawCircle(int16_t x, int16_t y, uint16_t r, uint16_t bg_color, bool fill);
    void drawInning(const Game* game);
    void topInningTriangle();
    void bottomInningTriangle();
    void drawBases(const Game* game);
    void drawBase(int16_t x, int16_t y, bool occupied);

public:
    /**
     * @brief Construct a new Game Drawer
     * @param display Pointer to the display matrix
     */
    explicit GameDrawer(MatrixPanel_I2S_DMA* display);
    ~GameDrawer();
    
    /**
     * @brief Draw the current game state
     * @param game Pointer to game data
     */
    void drawGame(const Game* game);
    
    /**
     * @brief Draw loading screen
     */
    void drawLoading();
    
    /**
     * @brief Draw WiFi connection screen
     */
    void drawWifi();
    
    /**
     * @brief Draw fullscreen text message
     * @param text Text to display
     */
    void drawFullscreenText(const String& text);
};

#endif // INCLUDE_GAMEDRAWER_H
