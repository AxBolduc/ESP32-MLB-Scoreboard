#ifndef INCLUDE_GAMEDRAWER_H
#define INCLUDE_GAMEDRAWER_H

#include "Matrix.h"
#include "Game.h"
#include "constants.h"

class GameDrawer
{
private:
    MatrixPanel_I2S_DMA* display = Matrix::get();
    void drawLayout(Game* game);
    void drawScores(Game* game);
    void drawBox(int16_t x, int16_t y, String text, uint16_t bg_color, uint16_t fg_color);
    void drawInning(Game* game);
    void topInningTriangle();
    void bottomInningTriangle();
    void drawBases(Game* game);
    void drawBase(int16_t x, int16_t y, bool occupied);
public:
    GameDrawer();
    ~GameDrawer();
    void drawGame(Game* game);
    void drawLoading();
};

GameDrawer::GameDrawer(/* args */)
{
}

GameDrawer::~GameDrawer()
{
}

void GameDrawer::drawGame(Game* game) {
    drawLayout(game);
    drawScores(game);
    drawBases(game);
}

void GameDrawer::drawLayout(Game* game)
{
    display->clearScreen();
    display->setTextColor(display->color565(255, 255, 255));

    int16_t upperLeftX, upperLeftY;
    uint16_t width, height;

    drawBox(0,0, game->getAwayTeam(), TEAM_BG_COLORS.find(game->getAwayTeamId())->second, TEAMS_TEXT_COLORS.find(game->getAwayTeamId())->second);
    drawInning(game);
    drawBox(MATRIX_WIDTH - 21, 0, game->getHomeTeam(), TEAM_BG_COLORS.find(game->getHomeTeamId())->second, TEAMS_TEXT_COLORS.find(game->getHomeTeamId())->second);
}

void GameDrawer::topInningTriangle(){
    display->fillTriangle(24, 7, 32, 7, 28, 3, display->color565(255, 255, 255));
}

void GameDrawer::bottomInningTriangle(){
    display->fillTriangle(24, 3, 32, 3, 28, 7, display->color565(255, 255, 255));
}

void GameDrawer::drawInning(Game* game) {
    if (game->isTopOfInning()) {
        topInningTriangle();
    } else {
        bottomInningTriangle();
    }
    display->setCursor(35, 2);
    display->print(game->getCurrentInning());
}

void GameDrawer::drawBox(int16_t x, int16_t y, String text, uint16_t bg_color, uint16_t fg_color) {
    int16_t upperLeftX, upperLeftY;
    uint16_t width, height;

    display->getTextBounds(text.c_str(), x+BOX_PADDING, y+BOX_PADDING, &upperLeftX, &upperLeftY, &width, &height);
    display->setCursor(upperLeftX, upperLeftY);

    // Draw Away team box - should always start at 0,0
    display->fillRect(x, y, width + 2 * BOX_PADDING - 1, height + 2 * BOX_PADDING - 1, bg_color);
    display->drawRect(x, y, width + 2 * BOX_PADDING - 1, height + 2 * BOX_PADDING - 1, fg_color);
    display->setTextColor(fg_color);
    display->print(text);
    display->setTextColor(COLORS::WHITE);
}

void GameDrawer::drawScores(Game* game){
    drawBox(6, 12, String(game->getAwayTeamScore()), COLORS::BLACK, COLORS::WHITE);

    drawBox(MATRIX_WIDTH - 15, 12, String(game->getHomeTeamScore()), COLORS::BLACK, COLORS::WHITE);
}

void GameDrawer::drawLoading(){
    display->clearScreen();
    display->setTextColor(display->color565(255, 255, 255));
    display->setCursor(0, 0);
    display->print("Loading...");
}

/**
 * @brief Draws a diamond base using two triangles
 * @param x X coordinate of the top verex of the base
 * @param y Y coordinate of the top verex of the base
 * @param occupied if the base is occupied
*/
void GameDrawer::drawBase(int16_t x, int16_t y, bool occupied){
    if (occupied) {
        display->fillTriangle(x, y, x - BASE_SIDE_LENGTH + 1, y + BASE_SIDE_LENGTH - 1, x, y + (2 * (BASE_SIDE_LENGTH-1)), COLORS::YELLOW);
        display->fillTriangle(x, y, x + BASE_SIDE_LENGTH - 1, y + BASE_SIDE_LENGTH - 1, x, y + (2 * (BASE_SIDE_LENGTH-1)), COLORS::YELLOW);
    } else {
        display->drawTriangle(x, y, x - BASE_SIDE_LENGTH + 1, y + BASE_SIDE_LENGTH - 1, x, y + (2 * (BASE_SIDE_LENGTH-1)), COLORS::YELLOW);
        display->drawTriangle(x, y, x + BASE_SIDE_LENGTH - 1, y + BASE_SIDE_LENGTH - 1, x, y + (2 * (BASE_SIDE_LENGTH-1)), COLORS::YELLOW);

        // Remove line in the middle of the base
        display->drawLine(x, y+1, x, y + (2 * (BASE_SIDE_LENGTH-1)) - 1, COLORS::BLACK);
    }
}

void GameDrawer::drawBases(Game* game){
    drawBase(BASES_TOP_X + BASE_SIDE_LENGTH + 1, BASES_TOP_Y + BASE_SIDE_LENGTH + 1, game->isFirstBaseOccupied());
    drawBase(BASES_TOP_X, BASES_TOP_Y, game->isSecondBaseOccupied());
    drawBase(BASES_TOP_X - BASE_SIDE_LENGTH - 1 , BASES_TOP_Y + BASE_SIDE_LENGTH + 1, game->isThirdBaseOccupied());
}

#endif // INCLUDE_GAMEDRAWER_H