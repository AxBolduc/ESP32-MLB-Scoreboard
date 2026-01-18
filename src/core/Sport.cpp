#include "Sport.h"
#include "../Config.h"

Sport::Sport(MatrixPanel_I2S_DMA *display)
    : display(display), drawer(nullptr), apiHandler(nullptr), currentGame(nullptr), currentTeamIndex(0)
{
    // Components will be created by subclasses via factory methods
}

Sport::~Sport()
{
    // Clean up owned resources
    if (currentGame)
    {
        delete currentGame;
        currentGame = nullptr;
    }

    if (drawer)
    {
        delete drawer;
        drawer = nullptr;
    }

    if (apiHandler)
    {
        delete apiHandler;
        apiHandler = nullptr;
    }

    // Don't delete display - we don't own it
}

void Sport::update()
{
    if (!apiHandler || !drawer)
    {
        Serial.println("ERROR: Sport components not initialized");
        return;
    }

    TeamInfo *teamInfo = getTeamInfo();
    if (!teamInfo)
    {
        Serial.println("ERROR: TeamInfo not available");
        return;
    }

    int teamId = teamInfo->getTeamId(currentTeamIndex);

    Serial.printf("Fetching schedule for team ID %d...\n", teamId);

    DynamicJsonDocument game(ApiConfig::JSON_BUFFER_SIZE);
    bool success = apiHandler->getTeamGame(teamInfo->getTeamAbbreviation(teamId), game);

    if (!success)
    {
        Serial.println("No Games");
        drawer->drawFullscreenText("No " + teamInfo->getTeamAbbreviation(teamId) + " games today");
        return;
    }

    // Check for games
    if (!game["sport"].is<JsonString>())
    {
        Serial.println("No games scheduled for today");
        String message = "No " + getSportName() + " game today";
        drawer->drawFullscreenText(message);
        return;
    }

    // Delete old game
    if (currentGame)
    {
        delete currentGame;
        currentGame = nullptr;
    }

    // Create and display new game
    currentGame = createGame(game.as<JsonObject>());
    if (currentGame)
    {
        drawer->drawGame(currentGame);
        Serial.println("Screen updated successfully");
    }
    else
    {
        Serial.println("ERROR: Failed to create game object");
    }
}

int Sport::nextTeam()
{
    TeamInfo *teamInfo = getTeamInfo();
    if (!teamInfo)
    {
        Serial.println("ERROR: TeamInfo not available");
        return -1;
    }

    currentTeamIndex = teamInfo->getNextTeamIndex(currentTeamIndex);
    Serial.printf("Switched to team index %d (ID: %d)\n",
                  currentTeamIndex, teamInfo->getTeamId(currentTeamIndex));

    return currentTeamIndex;
}

int Sport::previousTeam()
{
    TeamInfo *teamInfo = getTeamInfo();
    if (!teamInfo)
    {
        Serial.println("ERROR: TeamInfo not available");
        return -1;
    }

    currentTeamIndex = teamInfo->getPreviousTeamIndex(currentTeamIndex);
    Serial.printf("Switched to team index %d (ID: %d)\n",
                  currentTeamIndex, teamInfo->getTeamId(currentTeamIndex));

    return currentTeamIndex;
}

int Sport::getCurrentTeamId() const
{
    TeamInfo *teamInfo = const_cast<Sport *>(this)->getTeamInfo();
    if (!teamInfo)
    {
        return -1;
    }
    return teamInfo->getTeamId(currentTeamIndex);
}
