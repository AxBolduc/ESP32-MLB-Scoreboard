#include "UrlBuilder.h"
#include "Config.h"
#include <time.h>

String UrlBuilder::buildApiUrl(
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

String UrlBuilder::buildScheduleUrl(const String& date)
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

String UrlBuilder::buildTeamScheduleUrl(const String& date, TEAM_ID teamId)
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
    url += "&teamId=" + String(static_cast<int>(teamId));
    
    return url;
}

String UrlBuilder::getTodayDate()
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
