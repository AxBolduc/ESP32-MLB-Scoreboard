#include "NHLApiHandler.h"
#include "NHLUrlBuilder.h"
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

bool NHLApiHandler::getSchedule(const String& date, DynamicJsonDocument& outDoc)
{
    // STUBBED: NHL API not yet implemented
    // When ready, uncomment and implement:
    // String url = NHLUrlBuilder::buildScheduleUrl(date);
    // http.begin(url);
    // http.useHTTP10();
    // int httpCode = http.GET();
    // ... etc
    
    Serial.println("NHL API is stubbed - no game data available");
    return false;  // No game found
}

bool NHLApiHandler::getTeamSchedule(int teamId, const String& date, DynamicJsonDocument& outDoc)
{
    // STUBBED: NHL API not yet implemented
    // When ready, uncomment and implement:
    String url = NHLUrlBuilder::buildTeamScheduleUrl(date, teamId);
    http.begin(url);
    http.useHTTP10();
    int httpCode = http.GET();

    if(httpCode == 200){
        DeserializationError error = deserializeJson(outDoc, http.getStream());
        http.end();

        if(error) {
            Serial.printf("JSON parse error: %s\n", error.c_str());
            return false;
        }

        return true;
    } else {
        Serial.printf("HTTP error getting team schedule for team %d: %d\n", teamId, httpCode);
        http.end();
        return false;
    }
}

bool NHLApiHandler::getTeamScheduleToday(int teamId, DynamicJsonDocument& outDoc)
{
    // STUBBED: NHL API not yet implemented
    String date = NHLUrlBuilder::getTodayDate();
    if (date.length() == 0)
    {
        Serial.println("Failed to get today's date");
        return false;
    }
    
    return getTeamSchedule(teamId, date, outDoc);
}
