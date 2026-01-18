#include "NHLApiHandler.h"
#include "../../Config.h"

NHLApiHandler::NHLApiHandler() 
    : doc(ApiConfig::JSON_BUFFER_SIZE)
{
    // HTTPClient is default constructed
}

NHLApiHandler::~NHLApiHandler()
{
    // HTTPClient and DynamicJsonDocument are automatically cleaned up
}

bool NHLApiHandler::getTeamGame(String teamAbr, DynamicJsonDocument& outDoc) {
    String url = "https://" + String(NetworkConfig::WEBSOCKET_URL) + "/sports/nhl/game?teamAbr=" + teamAbr;

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

bool NHLApiHandler::getSchedule(const String& date, DynamicJsonDocument& outDoc)
{
    return true;
}

bool NHLApiHandler::getTeamSchedule(int teamId, const String& date, DynamicJsonDocument& outDoc)
{
    return true;
}

bool NHLApiHandler::getTeamScheduleToday(int teamId, DynamicJsonDocument& outDoc)
{
   return true;
}

