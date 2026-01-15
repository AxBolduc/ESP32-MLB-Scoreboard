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

void GameDrawer::drawFullscreenText(const String& text)
{
    if (!display) return;
    
    display->clearScreen();
    drawBox(0, 0, text, Color::BLACK, Color::WHITE);
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
