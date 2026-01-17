#ifndef INCLUDE_CORE_GAMEDRAWER_H
#define INCLUDE_CORE_GAMEDRAWER_H

#include <Arduino.h>
#include <ESP32-HUB75-MatrixPanel-I2S-DMA.h>
#include "Game.h"

/**
 * @brief Abstract base class for sport-specific game rendering
 * 
 * Provides shared drawing utilities and defines the interface for
 * sport-specific rendering implementations.
 */
class GameDrawer
{
protected:
    MatrixPanel_I2S_DMA* display;  // Injected, not owned
    
    /**
     * @brief Draw a box with text and colors
     * @param x X position
     * @param y Y position
     * @param text Text to display
     * @param bg_color Background color (RGB565)
     * @param fg_color Foreground/text color (RGB565)
     */
    void drawBox(int16_t x, int16_t y, const String& text, 
                uint16_t bg_color, uint16_t fg_color);
    
    /**
     * @brief Draw a circle
     * @param x Center X position
     * @param y Center Y position
     * @param r Radius
     * @param color Circle color (RGB565)
     * @param fill True to fill, false for outline only
     */
    void drawCircle(int16_t x, int16_t y, uint16_t r, 
                   uint16_t color, bool fill);
    
    /**
     * @brief Draw text with word wrapping and centering
     * @param text Text to wrap and display
     * @param maxWidth Maximum width in pixels before wrapping
     * @param fg_color Text color
     * @param bg_color Background color
     * @return Total height of rendered text block
     */
    uint16_t drawWrappedText(const String& text, uint16_t maxWidth, 
                            uint16_t fg_color, uint16_t bg_color);

public:
    /**
     * @brief Construct a new Game Drawer
     * @param display Pointer to the display matrix (not owned)
     */
    explicit GameDrawer(MatrixPanel_I2S_DMA* display);
    
    /**
     * @brief Virtual destructor
     */
    virtual ~GameDrawer() = default;
    
    /**
     * @brief Draw the current game state (sport-specific)
     * @param game Pointer to game data
     */
    virtual void drawGame(const Game* game) = 0;
    
    /**
     * @brief Draw loading screen (shared implementation)
     */
    void drawLoading();
    
    /**
     * @brief Draw WiFi connection screen (shared implementation)
     */
    void drawWifi();
    
    /**
     * @brief Draw fullscreen text message (shared implementation)
     * @param text Text to display
     */
    void drawFullscreenText(const String& text);
};

#endif // INCLUDE_CORE_GAMEDRAWER_H
