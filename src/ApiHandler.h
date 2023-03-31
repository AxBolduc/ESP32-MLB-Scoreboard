#ifndef INCLUDE_APIHANDLER_H
#define INCLUDE_APIHANDLER_H

#include "util.hpp"
#include "HTTPClient.h"
#include "ArduinoJson.h"

#define JSON_BUFFER_SIZE 4096

class ApiHandler
{
private:
    HTTPClient* http;
    DynamicJsonDocument* doc;
public:
    ApiHandler(HTTPClient* http, DynamicJsonDocument* doc);
    ~ApiHandler();
    JsonObject getSchedule(String date);
    JsonObject getTeamSchedule(String date, TEAM_ID team);
    JsonObject getTeamScheduleToday(TEAM_ID team);
};

ApiHandler::ApiHandler(HTTPClient* http, DynamicJsonDocument* doc)
{
    this->http = http;
    this->doc = doc;
}


ApiHandler::~ApiHandler()
{
    delete(http);
    delete(doc);
}

/**
 * @brief Get the schedule for a specific date
*/
JsonObject ApiHandler::getSchedule(String date)
{
    String url = scheduleUrlGenerator(date);
    Serial.println("URL GENERATED");
    http->begin(url);
    Serial.println("HTTP REQUEST STARTED");
    http->useHTTP10();
    int httpCode = http->GET();
    if (httpCode == 200)
    {
        deserializeJson(*doc, http->getStream());
        Serial.println("JSON DESERIALIZED");
    }
    else
    {
        Serial.println("Error on HTTP request");
        delete(this);
        exit(1);
    }
    Serial.println("HTTP REQUEST ENDED");
    http->end();
    return doc->as<JsonObject>();
}

/**
 * @brief Get the schedule for a specific team on a specific date
*/
JsonObject ApiHandler::getTeamSchedule(String date, TEAM_ID team)
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
        Serial.println("Error on HTTP request");
        delete(this);
        exit(1);
    }
    http->end();
    return doc->as<JsonObject>();
}

/**
 * @brief Get the schedule for a specific team today
*/
JsonObject ApiHandler::getTeamScheduleToday(TEAM_ID team)
{
    String url = scheduleUrlGenerator(getTodayDate(), team);

    Serial.printf("Requesting schedule at url: %s\n", url.c_str());

    http->begin(url);
    http->useHTTP10();
    int httpCode = http->GET();
    if (httpCode == 200)
    {
        deserializeJson(*doc, http->getStream());
    }
    else
    {
        Serial.println("Error on HTTP request");
        delete(this);
        exit(1);
    }
    http->end();
    return doc->as<JsonObject>();
}

#endif