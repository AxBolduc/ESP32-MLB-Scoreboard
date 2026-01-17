#include "Sport.h"
#include "../Config.h"

Sport::Sport(MatrixPanel_I2S_DMA* display)
    : display(display)
    , drawer(nullptr)
    , apiHandler(nullptr)
    , currentGame(nullptr)
    , currentTeamIndex(0)
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
    
    TeamInfo* teamInfo = getTeamInfo();
    if (!teamInfo)
    {
        Serial.println("ERROR: TeamInfo not available");
        return;
    }
    
    int teamId = teamInfo->getTeamId(currentTeamIndex);
    
    Serial.printf("Fetching schedule for team ID %d...\n", teamId);
    
    DynamicJsonDocument schedule(ApiConfig::JSON_BUFFER_SIZE);
    bool success = apiHandler->getTeamScheduleToday(teamId, schedule);
    
    if (!success)
    {
        Serial.println("Failed to get schedule");
        return;
    }
    
    // Check for games
    if (!schedule["dates"].is<JsonArray>() || schedule["dates"].size() == 0)
    {
        Serial.println("No games scheduled for today");
        String message = "No " + getSportName() + " game today";
        drawer->drawFullscreenText(message);
        return;
    }
    
    if (!schedule["dates"][0]["games"].is<JsonArray>() || 
        schedule["dates"][0]["games"].size() == 0)
    {
        Serial.println("No games in schedule");
        String message = "No " + getSportName() + " game data";
        drawer->drawFullscreenText(message);
        return;
    }
    
    JsonObject gameData = schedule["dates"][0]["games"][0].as<JsonObject>();
    
    // Delete old game
    if (currentGame)
    {
        delete currentGame;
        currentGame = nullptr;
    }
    
    // Create and display new game
    currentGame = createGame(gameData);
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

void Sport::nextTeam()
{
    TeamInfo* teamInfo = getTeamInfo();
    if (!teamInfo)
    {
        Serial.println("ERROR: TeamInfo not available");
        return;
    }
    
    currentTeamIndex = teamInfo->getNextTeamIndex(currentTeamIndex);
    Serial.printf("Switched to team index %d (ID: %d)\n", 
                  currentTeamIndex, teamInfo->getTeamId(currentTeamIndex));
}

void Sport::previousTeam()
{
    TeamInfo* teamInfo = getTeamInfo();
    if (!teamInfo)
    {
        Serial.println("ERROR: TeamInfo not available");
        return;
    }
    
    currentTeamIndex = teamInfo->getPreviousTeamIndex(currentTeamIndex);
    Serial.printf("Switched to team index %d (ID: %d)\n", 
                  currentTeamIndex, teamInfo->getTeamId(currentTeamIndex));
}

int Sport::getCurrentTeamId() const
{
    TeamInfo* teamInfo = const_cast<Sport*>(this)->getTeamInfo();
    if (!teamInfo)
    {
        return -1;
    }
    return teamInfo->getTeamId(currentTeamIndex);
}
