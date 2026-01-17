#include "NHLUrlBuilder.h"
#include "../../Config.h"
#include "./NHLConfig.h"
#include <time.h>

// STUBBED: NHL API endpoints not yet defined
// When ready, update BASE_URL in Config.h or use a different NHL API endpoint

String NHLUrlBuilder::buildScheduleUrl(const String& date)
{
    // STUBBED: Return empty URL - API not implemented
    // When ready, implement like this:
    // String url = String(NHLConfig::BASE_URL) + "schedule?date=" + date;
    // return url;
    
    Serial.println("NHL buildScheduleUrl is stubbed");
    return "";
}

String NHLUrlBuilder::buildTeamScheduleUrl(const String& date, int teamId)
{
    // TODO: Get 3-letter team abbreviation from teamId
    String url = String(NHLConfig::BASE_URL) + "v1/club-schedule/" + "BOS" + "/week/now";
    
    return url;
}

String NHLUrlBuilder::getTodayDate()
{
    // Reuse time logic from MLB (this works for any sport)
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
