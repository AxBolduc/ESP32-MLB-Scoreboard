#ifndef INCLUDE_APIHANDLER_H
#define INCLUDE_APIHANDLER_H

#include "util.hpp"
#include "HTTPClient.h"
#include "ArduinoJson.h"

class ApiHandler
{
private:
    HTTPClient *http;
    DynamicJsonDocument *doc;

public:
    ApiHandler(HTTPClient *http, DynamicJsonDocument *doc);
    ~ApiHandler();
    JsonObject getSchedule(String date);
    JsonObject getTeamSchedule(TEAM_ID team, String date);
    DynamicJsonDocument *getTeamScheduleToday(TEAM_ID team);
};

ApiHandler::ApiHandler(HTTPClient *http, DynamicJsonDocument *doc)
{
    this->http = http;
    this->doc = doc;
}

ApiHandler::~ApiHandler()
{
    delete (http);
    delete (doc);
}

/**
 * @brief Get the schedule for a specific date
 */
JsonObject ApiHandler::getSchedule(String date)
{
    String url = scheduleUrlGenerator(date);
    http->begin(url);
    http->useHTTP10();
    int httpCode = http->GET();
    if (httpCode == 200)
    {
        deserializeJson(*doc, http->getStream());
    }
    else
    {
        Serial.printf("Error getting mlb schedule for date (%s)", date);
    }
    http->end();
    return doc->as<JsonObject>();
}

/**
 * @brief Get the schedule for a specific team on a specific date
 */
JsonObject ApiHandler::getTeamSchedule(TEAM_ID team, String date)
{
    String url = scheduleUrlGenerator(date, team);
    http->begin(url);
    http->useHTTP10();
    int httpCode = http->GET();
    if (httpCode == 200)
    {
        deserializeJson(*doc, http->getStream());
    }
    else
    {
        Serial.printf("Error getting %s schedule for date (%s)\n", team, date);
        delete (this);
        exit(1);
    }
    http->end();
    return doc->as<JsonObject>();
}

/**
 * @brief Get the schedule for a specific team today
 */
DynamicJsonDocument *ApiHandler::getTeamScheduleToday(TEAM_ID team)
{
    String url = scheduleUrlGenerator(getTodayDate(), team);

    http->begin(url);
    http->useHTTP10();
    int httpCode = http->GET();
    if (httpCode == 200)
    {
        deserializeJson(*doc, http->getStream());
    }
    else
    {
        Serial.printf("Error getting %s schedule\n", team);
        Serial.println(httpCode);
    }
    http->end();
    // return doc->as<JsonObject>();
    return doc;
}

#endif