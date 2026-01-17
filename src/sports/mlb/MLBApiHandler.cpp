#include "MLBApiHandler.h"
#include "MLBUrlBuilder.h"
#include "../../Config.h"

MLBApiHandler::MLBApiHandler() 
    : doc(ApiConfig::JSON_BUFFER_SIZE)
{
    // HTTPClient is default constructed
}

MLBApiHandler::~MLBApiHandler()
{
    // HTTPClient and DynamicJsonDocument are automatically cleaned up
}

bool MLBApiHandler::getTeamGame(String teamAbr, DynamicJsonDocument& outDoc) {
    String url = "https://" + String(NetworkConfig::WEBSOCKET_URL) + "/sports/mlb/game?teamAbr=" + teamAbr;

    Serial.printf("Fetching team game from URL: %s\n", url.c_str());

    http.begin(url);
    http.useHTTP10();

    int httpCode = http.GET();
    if (httpCode == 200)
    {
        DeserializationError error = deserializeJson(outDoc, http.getStream());
        http.end();

        if (error)
        {
            Serial.printf("JSON parse error: %s\n", error.c_str());
            return false;
        }
        return true;
    }
    else
    {
        Serial.printf("HTTP error getting team game: %d\n", httpCode);
        http.end();
        return false;
    }
}

bool MLBApiHandler::getSchedule(const String& date, DynamicJsonDocument& outDoc)
{
    String url = MLBUrlBuilder::buildScheduleUrl(date);
    http.begin(url);
    http.useHTTP10();
    
    int httpCode = http.GET();
    if (httpCode == 200)
    {
        DeserializationError error = deserializeJson(outDoc, http.getStream());
        http.end();
        
        if (error)
        {
            Serial.print("JSON deserialization failed: ");
            Serial.println(error.c_str());
            return false;
        }
        return true;
    }
    else
    {
        Serial.printf("HTTP error getting schedule for date %s: %d\n", date.c_str(), httpCode);
        http.end();
        return false;
    }
}

bool MLBApiHandler::getTeamSchedule(int teamId, const String& date, DynamicJsonDocument& outDoc)
{
    String url = MLBUrlBuilder::buildTeamScheduleUrl(date, teamId);
    http.begin(url);
    http.useHTTP10();
    
    int httpCode = http.GET();
    if (httpCode == 200)
    {
        DeserializationError error = deserializeJson(outDoc, http.getStream());
        http.end();
        
        if (error)
        {
            Serial.print("JSON deserialization failed: ");
            Serial.println(error.c_str());
            return false;
        }
        return true;
    }
    else
    {
        Serial.printf("HTTP error getting team %d schedule for date %s: %d\n", 
                     teamId, date.c_str(), httpCode);
        http.end();
        return false;
    }
}

bool MLBApiHandler::getTeamScheduleToday(int teamId, DynamicJsonDocument& outDoc)
{
    String date = MLBUrlBuilder::getTodayDate();
    if (date.length() == 0)
    {
        Serial.println("Failed to get today's date");
        return false;
    }
    
    return getTeamSchedule(teamId, date, outDoc);
}
