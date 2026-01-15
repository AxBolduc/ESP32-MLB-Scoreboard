#include "GameDrawer.h"
#include "Config.h"
#include "TeamInfo.h"

GameDrawer::GameDrawer(MatrixPanel_I2S_DMA* display)
    : display(display)
{
    // Display is injected, not owned
}

GameDrawer::~GameDrawer()
{
    // Don't delete display - we don't own it
}

void GameDrawer::drawGame(const Game* game)
{
    if (!game || !display) return;
    
    drawLayout(game);
    drawScores(game);
    drawBases(game);
    drawCount(game);
}

void GameDrawer::drawCount(const Game* game)
{
    if (!game || !display) return;
    
    const uint16_t dotRadius = 2;
    int balls = game->getBalls();
    int strikes = game->getStrikes();
    int outs = game->getOuts();

    // Balls
    drawCircle(3, 27, dotRadius, Color::GREEN, balls >= 1);
    drawCircle(10, 27, dotRadius, Color::GREEN, balls >= 2);
    drawCircle(17, 27, dotRadius, Color::GREEN, balls >= 3);

    // Strikes
    drawCircle(28, 27, dotRadius, Color::RED, strikes >= 1);
    drawCircle(36, 27, dotRadius, Color::RED, strikes >= 2);

    // Outs
    drawCircle(49, 27, dotRadius, Color::RED, outs >= 1);
    drawCircle(57, 27, dotRadius, Color::RED, outs >= 2);
}

void GameDrawer::drawLayout(const Game* game)
{
    if (!game || !display) return;
    
    display->clearScreen();
    display->setTextColor(display->color565(255, 255, 255));

    int awayTeamId = game->getAwayTeamId();
    int homeTeamId = game->getHomeTeamId();
    
    drawBox(0, 0, game->getAwayTeam(), 
            TeamInfo::getBackgroundColor(awayTeamId),
            TeamInfo::getTextColor(awayTeamId));
    
    drawInning(game);
    
    drawBox(DisplayConfig::DISPLAY_WIDTH - 21, 0, game->getHomeTeam(), 
            TeamInfo::getBackgroundColor(homeTeamId),
            TeamInfo::getTextColor(homeTeamId));
}

void GameDrawer::topInningTriangle()
{
    if (!display) return;
    display->fillTriangle(24, 7, 32, 7, 28, 3, display->color565(255, 255, 255));
}

void GameDrawer::bottomInningTriangle()
{
    if (!display) return;
    display->fillTriangle(24, 3, 32, 3, 28, 7, display->color565(255, 255, 255));
}

void GameDrawer::drawInning(const Game* game)
{
    if (!game || !display) return;
    
    if (game->isTopOfInning())
    {
        topInningTriangle();
    }
    else
    {
        bottomInningTriangle();
    }
    display->setCursor(35, 2);
    display->print(game->getCurrentInning());
}

void GameDrawer::drawBox(int16_t x, int16_t y, const String& text, uint16_t bg_color, uint16_t fg_color)
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

void GameDrawer::drawCircle(int16_t x, int16_t y, uint16_t r, uint16_t bg_color, bool fill)
{
    if (!display) return;
    
    if (fill)
    {
        display->fillCircle(x, y, r, bg_color);
    }
    else
    {
        display->drawCircle(x, y, r, bg_color);
    }
}

void GameDrawer::drawScores(const Game* game)
{
    if (!game || !display) return;
    
    const uint16_t scoreBoxMarginFromEdge = 6;

    drawBox(0 + scoreBoxMarginFromEdge, 12, String(game->getAwayTeamScore()), 
           Color::BLACK, Color::WHITE);

    drawBox(DisplayConfig::DISPLAY_WIDTH - scoreBoxMarginFromEdge - 9, 12, 
           String(game->getHomeTeamScore()), Color::BLACK, Color::WHITE);
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

void GameDrawer::drawBase(int16_t x, int16_t y, bool occupied)
{
    if (!display) return;
    
    if (occupied)
    {
        display->fillTriangle(x, y, x - DisplayConfig::BASE_SIDE_LENGTH + 1, 
                            y + DisplayConfig::BASE_SIDE_LENGTH - 1, 
                            x, y + (2 * (DisplayConfig::BASE_SIDE_LENGTH - 1)), 
                            Color::YELLOW);
        display->fillTriangle(x, y, x + DisplayConfig::BASE_SIDE_LENGTH - 1, 
                            y + DisplayConfig::BASE_SIDE_LENGTH - 1, 
                            x, y + (2 * (DisplayConfig::BASE_SIDE_LENGTH - 1)), 
                            Color::YELLOW);
    }
    else
    {
        display->drawTriangle(x, y, x - DisplayConfig::BASE_SIDE_LENGTH + 1, 
                            y + DisplayConfig::BASE_SIDE_LENGTH - 1, 
                            x, y + (2 * (DisplayConfig::BASE_SIDE_LENGTH - 1)), 
                            Color::YELLOW);
        display->drawTriangle(x, y, x + DisplayConfig::BASE_SIDE_LENGTH - 1, 
                            y + DisplayConfig::BASE_SIDE_LENGTH - 1, 
                            x, y + (2 * (DisplayConfig::BASE_SIDE_LENGTH - 1)), 
                            Color::YELLOW);

        // Remove line in the middle of the base
        display->drawLine(x, y + 1, x, y + (2 * (DisplayConfig::BASE_SIDE_LENGTH - 1)) - 1, 
                         Color::BLACK);
    }
}

void GameDrawer::drawBases(const Game* game)
{
    if (!game || !display) return;
    
    drawBase(DisplayConfig::BASES_TOP_X + DisplayConfig::BASE_SIDE_LENGTH + 1, 
            DisplayConfig::BASES_TOP_Y + DisplayConfig::BASE_SIDE_LENGTH + 1, 
            game->isFirstBaseOccupied());
    drawBase(DisplayConfig::BASES_TOP_X, DisplayConfig::BASES_TOP_Y, 
            game->isSecondBaseOccupied());
    drawBase(DisplayConfig::BASES_TOP_X - DisplayConfig::BASE_SIDE_LENGTH - 1, 
            DisplayConfig::BASES_TOP_Y + DisplayConfig::BASE_SIDE_LENGTH + 1, 
            game->isThirdBaseOccupied());
}
