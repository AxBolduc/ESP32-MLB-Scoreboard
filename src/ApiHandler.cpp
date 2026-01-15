#include "ApiHandler.h"
#include "UrlBuilder.h"
#include "Config.h"

ApiHandler::ApiHandler() 
    : doc(ApiConfig::JSON_BUFFER_SIZE)
{
    // HTTPClient is default constructed
}

ApiHandler::~ApiHandler()
{
    // HTTPClient and DynamicJsonDocument are automatically cleaned up
}

bool ApiHandler::getSchedule(const String& date, DynamicJsonDocument& outDoc)
{
    String url = UrlBuilder::buildScheduleUrl(date);
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

bool ApiHandler::getTeamSchedule(TEAM_ID team, const String& date, DynamicJsonDocument& outDoc)
{
    String url = UrlBuilder::buildTeamScheduleUrl(date, team);
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
                     static_cast<int>(team), date.c_str(), httpCode);
        http.end();
        return false;
    }
}

bool ApiHandler::getTeamScheduleToday(TEAM_ID team, DynamicJsonDocument& outDoc)
{
    String date = UrlBuilder::getTodayDate();
    if (date.length() == 0)
    {
        Serial.println("Failed to get today's date");
        return false;
    }
    
    return getTeamSchedule(team, date, outDoc);
}
