#ifndef INCLUDE_UTIL_H
#define INCLUDE_UTIL_H

#include <Arduino.h>
#include "./constants.h"
#include "time.h"

String apiURLGenerator(String endpoint, String hydrations[HYDRATIONS_ARRAY_SIZE], String fields[])
{
    String url = BASE_URL + String("v1/") + endpoint + String("?sportId=1");
    url += String("&hydrate=");
    for (int i = 0; i < HYDRATIONS_ARRAY_SIZE; i++)
    {
        url += hydrations[i];
        if (i < HYDRATIONS_ARRAY_SIZE - 1)
            if (hydrations[i] != "")
            {
                url += String(",");
            }
            else
            {
                break;
            }
    }

    url += String("&fields=");
    for (int i = 0; i < FIELDS_ARRAY_SIZE; i++)
    {
        url += fields[i];
        if (i < FIELDS_ARRAY_SIZE - 1)
            if (fields[i] != "")
            {
                url += String(",");
            }
            else
            {
                break;
            }
    }
    return url;
}

String scheduleUrlGenerator(String date)
{
    String hydrations[HYDRATIONS_ARRAY_SIZE] = {"team", "linescore"};
    String fields[FIELDS_ARRAY_SIZE] = {
        "dates",
        "games",
        "teams",
        "team",
        "name",
        "abbreviation",
        "linescore",
        "home",
        "away",
        "runs"};

    String url = apiURLGenerator("schedule", hydrations, fields);

    url += String("&startDate=");
    url += date;
    url += String("&endDate=");
    url += date;

    Serial.println(url);

    return url;
}

String scheduleUrlGenerator(String date, TEAM_ID teamId)
{
    String hydrations[HYDRATIONS_ARRAY_SIZE] = {"team", "linescore"};
    String fields[FIELDS_ARRAY_SIZE] = {"dates",
                                        "games",
                                        "teams",
                                        "team",
                                        "id",
                                        "name",
                                        "abbreviation",
                                        "linescore",
                                        "home",
                                        "away",
                                        "runs",
                                        "currentInning",
                                        "isTopInning",
                                        "offense",
                                        "first",
                                        "second",
                                        "third"
                                        };

    String url = apiURLGenerator("schedule", hydrations, fields);

    url += String("&startDate=");
    url += date;
    url += String("&endDate=");
    url += date;
    url += String("&teamId=");
    url += teamId;

    return url;
}

String getTodayDate()
{
    struct tm timeInfo;
    if (!getLocalTime(&timeInfo))
    {
        Serial.println("Failed to obtain time");
        return "";
    }

    String date = String(timeInfo.tm_year + 1900);
    date += String("-");
    date += String(timeInfo.tm_mon + 1);
    date += String("-");
    date += String(timeInfo.tm_mday);
    return date;
}

#endif