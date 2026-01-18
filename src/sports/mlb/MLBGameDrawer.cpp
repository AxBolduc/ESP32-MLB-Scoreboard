#include "MLBGameDrawer.h"
#include "../../Config.h"
#include "MLBTeamInfo.h"

// Color enum for MLB-specific rendering
enum Color : uint16_t
{
    RED = 0x8000,
    YELLOW = 0xFFE0,
    WHITE = 0xFFFF,
    BLACK = 0x0000,
    GREEN = 0x07E0
};

MLBGameDrawer::MLBGameDrawer(MatrixPanel_I2S_DMA* display)
    : GameDrawer(display)
{
    // Base class handles display injection
}

MLBGameDrawer::~MLBGameDrawer()
{
    // Base class handles cleanup
}

void MLBGameDrawer::drawGame(const Game* game)
{
    if (!game || !display) return;
    
    // Cast to MLB-specific game for baseball data access
    // Safe cast since we know this drawer only receives MLBGame objects
    const MLBGame* mlbGame = static_cast<const MLBGame*>(game);
    
    drawLayout(mlbGame);
    drawScores(mlbGame);
    drawBases(mlbGame);
    drawCount(mlbGame);
}

void MLBGameDrawer::drawCount(const MLBGame* game)
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

void MLBGameDrawer::drawLayout(const MLBGame* game)
{
    if (!game || !display) return;
    
    display->clearScreen();
    display->setTextColor(display->color565(255, 255, 255));

    int awayTeamId = game->getAwayTeamId();
    int homeTeamId = game->getHomeTeamId();
    
    // Create temporary team info instance for color lookups
    MLBTeamInfo teamInfo;
    
    drawBox(0, 0, game->getAwayTeam(), 
            teamInfo.getBackgroundColor(awayTeamId),
            teamInfo.getTextColor(awayTeamId));
    
    drawInning(game);
    
    drawBox(DisplayConfig::DISPLAY_WIDTH - 21, 0, game->getHomeTeam(), 
            teamInfo.getBackgroundColor(homeTeamId),
            teamInfo.getTextColor(homeTeamId));
}

void MLBGameDrawer::topInningTriangle()
{
    if (!display) return;
    display->fillTriangle(24, 7, 32, 7, 28, 3, display->color565(255, 255, 255));
}

void MLBGameDrawer::bottomInningTriangle()
{
    if (!display) return;
    display->fillTriangle(24, 3, 32, 3, 28, 7, display->color565(255, 255, 255));
}

void MLBGameDrawer::drawInning(const MLBGame* game)
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

void MLBGameDrawer::drawScores(const MLBGame* game)
{
    if (!game || !display) return;
    
    const uint16_t scoreBoxMarginFromEdge = 6;

    drawBox(0 + scoreBoxMarginFromEdge, 12, String(game->getAwayTeamScore()), 
           Color::BLACK, Color::WHITE);

    drawBox(DisplayConfig::DISPLAY_WIDTH - scoreBoxMarginFromEdge - 9, 12, 
           String(game->getHomeTeamScore()), Color::BLACK, Color::WHITE);
}

void MLBGameDrawer::drawBase(int16_t x, int16_t y, bool occupied)
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

void MLBGameDrawer::drawBases(const MLBGame* game)
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
