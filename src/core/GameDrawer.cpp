#include "GameDrawer.h"
#include "../Config.h"

// Color enum for shared use
enum Color : uint16_t
{
    RED = 0x8000,
    YELLOW = 0xFFE0,
    WHITE = 0xFFFF,
    BLACK = 0x0000,
    NAVY_BLUE = 0x000F,
    ROYAL_BLUE = 0x435C,
    ORANGE = 0xFD20,
    GREEN = 0x07E0,
    GRAY = 0x8410,
    BROWN = 0xA145,
    PURPLE = 0x8010
};

GameDrawer::GameDrawer(MatrixPanel_I2S_DMA* display)
    : display(display)
{
    // Display is injected, not owned
}

void GameDrawer::drawBox(int16_t x, int16_t y, const String& text, 
                        uint16_t bg_color, uint16_t fg_color)
{
    if (!display) return;
    
    int16_t upperLeftX, upperLeftY;
    uint16_t width, height;

    display->getTextBounds(text.c_str(), x + DisplayConfig::BOX_PADDING, y + DisplayConfig::BOX_PADDING, 
                          &upperLeftX, &upperLeftY, &width, &height);
    display->setCursor(upperLeftX, upperLeftY);

    // Draw box
    display->fillRect(x, y, width + 2 * DisplayConfig::BOX_PADDING - 1, 
                     height + 2 * DisplayConfig::BOX_PADDING - 1, bg_color);
    display->drawRect(x, y, width + 2 * DisplayConfig::BOX_PADDING - 1, 
                     height + 2 * DisplayConfig::BOX_PADDING - 1, fg_color);
    display->setTextColor(fg_color);
    display->print(text);
    display->setTextColor(Color::WHITE);
}

void GameDrawer::drawCircle(int16_t x, int16_t y, uint16_t r, 
                           uint16_t color, bool fill)
{
    if (!display) return;
    
    if (fill)
    {
        display->fillCircle(x, y, r, color);
    }
    else
    {
        display->drawCircle(x, y, r, color);
    }
}

void GameDrawer::drawLoading()
{
    if (!display) return;
    
    display->clearScreen();
    display->setTextColor(display->color565(255, 255, 255));
    display->setCursor(0, 0);
    display->print("Loading...");
}

void GameDrawer::drawWifi()
{
    drawFullscreenText("Connect to WIFI");
}

uint16_t GameDrawer::drawWrappedText(const String& text, uint16_t maxWidth, 
                                     uint16_t fg_color, uint16_t bg_color)
{
    if (!display || text.length() == 0) return 0;
    
    // Array to hold wrapped lines (max 10 lines)
    String lines[10];
    int lineCount = 0;
    
    // Split text into words and build lines
    String currentLine = "";
    int wordStart = 0;
    int textLen = text.length();
    
    for (int i = 0; i <= textLen; i++)
    {
        // Check if we're at a space or end of string
        if (i == textLen || text[i] == ' ')
        {
            // Extract the word
            String word = text.substring(wordStart, i);
            wordStart = i + 1;
            
            if (word.length() == 0) continue;
            
            // Build test string (current line + space + word)
            String testLine = currentLine;
            if (testLine.length() > 0) testLine += " ";
            testLine += word;
            
            // Measure the test line
            int16_t x1, y1;
            uint16_t w, h;
            display->getTextBounds(testLine.c_str(), 0, 0, &x1, &y1, &w, &h);
            
            // Check if word itself is too long
            display->getTextBounds(word.c_str(), 0, 0, &x1, &y1, &w, &h);
            if (w > maxWidth)
            {
                // Truncate word with ellipsis
                String truncated = word;
                while (truncated.length() > 3)
                {
                    truncated = truncated.substring(0, truncated.length() - 1);
                    String withEllipsis = truncated + "...";
                    display->getTextBounds(withEllipsis.c_str(), 0, 0, &x1, &y1, &w, &h);
                    if (w <= maxWidth)
                    {
                        word = withEllipsis;
                        break;
                    }
                }
            }
            
            // Now check if adding word to current line exceeds width
            testLine = currentLine;
            if (testLine.length() > 0) testLine += " ";
            testLine += word;
            display->getTextBounds(testLine.c_str(), 0, 0, &x1, &y1, &w, &h);
            
            if (w <= maxWidth)
            {
                // Word fits on current line
                if (currentLine.length() > 0) currentLine += " ";
                currentLine += word;
            }
            else
            {
                // Word doesn't fit, save current line and start new one
                if (currentLine.length() > 0 && lineCount < DisplayConfig::TEXT_MAX_LINES)
                {
                    lines[lineCount++] = currentLine;
                }
                currentLine = word;
            }
        }
    }
    
    // Add the last line
    if (currentLine.length() > 0 && lineCount < DisplayConfig::TEXT_MAX_LINES)
    {
        lines[lineCount++] = currentLine;
    }
    
    // Calculate text dimensions
    int16_t x1, y1;
    uint16_t w, h;
    display->getTextBounds("A", 0, 0, &x1, &y1, &w, &h);
    uint16_t lineHeight = h;
    uint16_t totalHeight = (lineCount * lineHeight) + ((lineCount - 1) * DisplayConfig::TEXT_LINE_SPACING);
    
    // Calculate starting Y position to center vertically
    int16_t startY = (DisplayConfig::PANEL_HEIGHT - totalHeight) / 2;
    if (startY < DisplayConfig::BOX_PADDING) startY = DisplayConfig::BOX_PADDING;
    
    // Calculate box dimensions for background
    uint16_t maxLineWidth = 0;
    for (int i = 0; i < lineCount; i++)
    {
        display->getTextBounds(lines[i].c_str(), 0, 0, &x1, &y1, &w, &h);
        if (w > maxLineWidth) maxLineWidth = w;
    }
    
    // Draw background box
    int16_t boxX = (DisplayConfig::DISPLAY_WIDTH - maxLineWidth) / 2 - DisplayConfig::BOX_PADDING;
    int16_t boxY = startY - DisplayConfig::BOX_PADDING;
    uint16_t boxWidth = maxLineWidth + 2 * DisplayConfig::BOX_PADDING;
    uint16_t boxHeight = totalHeight + 2 * DisplayConfig::BOX_PADDING;
    
    display->fillRect(boxX, boxY, boxWidth, boxHeight, bg_color);
    display->drawRect(boxX, boxY, boxWidth, boxHeight, fg_color);
    
    // Draw each line centered
    int16_t currentY = startY;
    display->setTextColor(fg_color);
    
    for (int i = 0; i < lineCount; i++)
    {
        // Measure line width for centering
        display->getTextBounds(lines[i].c_str(), 0, 0, &x1, &y1, &w, &h);
        int16_t centerX = (DisplayConfig::DISPLAY_WIDTH - w) / 2;
        
        display->setCursor(centerX, currentY);
        display->print(lines[i]);
        
        currentY += lineHeight + DisplayConfig::TEXT_LINE_SPACING;
    }
    
    display->setTextColor(Color::WHITE);
    return totalHeight;
}

void GameDrawer::drawFullscreenText(const String& text)
{
    if (!display) return;
    
    display->clearScreen();
    
    // Calculate max text width (display width minus padding and border)
    uint16_t maxWidth = DisplayConfig::DISPLAY_WIDTH - (2 * DisplayConfig::BOX_PADDING) - 2;
    
    // Draw wrapped text with box
    drawWrappedText(text, maxWidth, Color::WHITE, Color::BLACK);
}
