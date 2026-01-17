#include "MLBUrlBuilder.h"
#include "../../Config.h"
#include <time.h>

String MLBUrlBuilder::buildApiUrl(
    const String& endpoint,
    const std::vector<String>& hydrations,
    const std::vector<String>& fields)
{
    String url = String(NetworkConfig::BASE_URL) + "v1/" + endpoint + "?sportId=1";
    
    // Add hydrations
    url += "&hydrate=";
    for (size_t i = 0; i < hydrations.size(); i++)
    {
        url += hydrations[i];
        if (i < hydrations.size() - 1)
        {
            url += ",";
        }
    }
    
    // Add fields
    url += "&fields=";
    for (size_t i = 0; i < fields.size(); i++)
    {
        url += fields[i];
        if (i < fields.size() - 1)
        {
            url += ",";
        }
    }
    
    return url;
}

String MLBUrlBuilder::buildScheduleUrl(const String& date)
{
    std::vector<String> hydrations = {"team", "linescore"};
    std::vector<String> fields = {
        "dates", "games", "teams", "team",
        "name", "abbreviation", "linescore",
        "home", "away", "runs"
    };
    
    String url = buildApiUrl("schedule", hydrations, fields);
    url += "&startDate=" + date;
    url += "&endDate=" + date;
    
    Serial.println(url);
    return url;
}

String MLBUrlBuilder::buildTeamScheduleUrl(const String& date, int teamId)
{
    std::vector<String> hydrations = {"team", "linescore"};
    std::vector<String> fields = {
        "dates", "games", "teams", "team",
        "id", "name", "abbreviation", "linescore",
        "home", "away", "runs", "currentInning",
        "isTopInning", "offense", "first", "second",
        "third", "balls", "strikes", "outs"
    };
    
    String url = buildApiUrl("schedule", hydrations, fields);
    url += "&startDate=" + date;
    url += "&endDate=" + date;
    url += "&teamId=" + String(teamId);
    
    return url;
}

String MLBUrlBuilder::getTodayDate()
{
    struct tm timeInfo;
    if (!getLocalTime(&timeInfo))
    {
        Serial.println("Failed to obtain time");
        return "";
    }
    
    String date = String(timeInfo.tm_year + 1900);
    date += "-";
    if (timeInfo.tm_mon + 1 < 10) date += "0";
    date += String(timeInfo.tm_mon + 1);
    date += "-";
    if (timeInfo.tm_mday < 10) date += "0";
    date += String(timeInfo.tm_mday);
    
    return date;
}
