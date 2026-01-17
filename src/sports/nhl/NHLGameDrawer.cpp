#include "NHLGameDrawer.h"
#include "../../Config.h"
#include "NHLTeamInfo.h"

// Color enum for NHL-specific rendering
enum Color : uint16_t
{
    RED = 0x8000,
    YELLOW = 0xFFE0,
    WHITE = 0xFFFF,
    BLACK = 0x0000,
    BLUE = 0x001F,
    CYAN = 0x07FF
};

NHLGameDrawer::NHLGameDrawer(MatrixPanel_I2S_DMA* display)
    : GameDrawer(display)
{
    // Base class handles display injection
}

NHLGameDrawer::~NHLGameDrawer()
{
    // Base class handles cleanup
}

void NHLGameDrawer::drawGame(const Game* game)
{
    if (!game || !display) return;
    
    // Cast to NHL-specific game for hockey data access
    // Safe cast since we know this drawer only receives NHLGame objects
    const NHLGame* nhlGame = static_cast<const NHLGame*>(game);
    
    drawLayout(nhlGame);
    drawScores(nhlGame);
    drawPeriod(nhlGame);
    drawTime(nhlGame);
    drawShots(nhlGame);
    drawOvertimeIndicator(nhlGame);
}

void NHLGameDrawer::drawLayout(const NHLGame* game)
{
    if (!game || !display) return;
    
    display->clearScreen();
    display->setTextColor(display->color565(255, 255, 255));

    int awayTeamId = game->getAwayTeamId();
    int homeTeamId = game->getHomeTeamId();
    
    // Create temporary team info instance for color lookups
    NHLTeamInfo teamInfo;
    
    // Draw team abbreviations (3 letters) at top
    drawBox(0, 0, game->getAwayTeam(), 
            teamInfo.getBackgroundColor(awayTeamId),
            teamInfo.getTextColor(awayTeamId));
    
    drawBox(DisplayConfig::DISPLAY_WIDTH - 21, 0, game->getHomeTeam(), 
            teamInfo.getBackgroundColor(homeTeamId),
            teamInfo.getTextColor(homeTeamId));
}

void NHLGameDrawer::drawScores(const NHLGame* game)
{
    if (!game || !display) return;
    
    const uint16_t scoreBoxMarginFromEdge = 6;

    // Draw scores below team names
    drawBox(0 + scoreBoxMarginFromEdge, 12, String(game->getAwayTeamScore()), 
           Color::BLACK, Color::WHITE);

    drawBox(DisplayConfig::DISPLAY_WIDTH - scoreBoxMarginFromEdge - 9, 12, 
           String(game->getHomeTeamScore()), Color::BLACK, Color::WHITE);
}

void NHLGameDrawer::drawPeriod(const NHLGame* game)
{
    if (!game || !display) return;
    
    // Draw period indicator in center-top area
    display->setCursor(26, 2);
    display->setTextColor(Color::WHITE);
    
    int period = game->getCurrentPeriod();
    
    // Display "P1", "P2", "P3" for regulation
    // "OT" for overtime, "SO" for shootout
    if (game->getIsShootout())
    {
        display->print("SO");
    }
    else if (game->getIsOvertime())
    {
        display->print("OT");
    }
    else
    {
        display->print("P");
        display->print(period);
    }
}

void NHLGameDrawer::drawTime(const NHLGame* game)
{
    if (!game || !display) return;
    
    // Draw time remaining in small text at bottom center
    String timeStr = game->getTimeRemaining();
    
    display->setTextColor(Color::CYAN);
    display->setCursor(22, 23);
    display->setTextSize(1);
    display->print(timeStr);
}

void NHLGameDrawer::drawShots(const NHLGame* game)
{
    if (!game || !display) return;
    
    // Draw shots on goal as small numbers below scores
    display->setTextColor(Color::YELLOW);
    display->setTextSize(1);
    
    // Away team shots
    display->setCursor(8, 20);
    display->print(game->getAwayTeamShots());
    
    // Home team shots
    display->setCursor(DisplayConfig::DISPLAY_WIDTH - 14, 20);
    display->print(game->getHomeTeamShots());
}

void NHLGameDrawer::drawOvertimeIndicator(const NHLGame* game)
{
    if (!game || !display) return;
    
    // Draw OT/SO indicator near center if applicable
    // (This is handled in drawPeriod, so this method is optional/reserved)
    // Could add a visual indicator like a small dot or icon here if desired
}
