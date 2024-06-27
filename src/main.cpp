// #include <ESP32-HUB75-MatrixPanel-I2S-DMA.h>
#include "constants.h"
#include "WiFi.h"
#include "ApiHandler.h"
#include "Matrix.h"
#include "GameDrawer.h"
#include "./util.hpp"
#include <WiFiManager.h>
#include "./touchButton.h"

MatrixPanel_I2S_DMA *Matrix::dma_display = nullptr;
MatrixPanel_I2S_DMA *dma_display;

unsigned long time_counter = 0;
ApiHandler *apiHandler;

// Define a score struct
struct Score
{
  int home;
  int away;
};

Score currentScore;

char ssid[] = "HowAreYouDoing";
char password[] = "livingthedream";

const char *ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 3600 * -1 * 5;
const int daylightOffset_sec = 3600;

int status = WL_IDLE_STATUS;

TEAM_ID teamId = TEAM_ID::BOSTON_REDSOX;

HTTPClient http;
DynamicJsonDocument doc(JSON_BUFFER_SIZE);
GameDrawer gameDrawer;

TouchButtonConfig touchConfig(T9);
ace_button::AceButton aceButton(&touchConfig, T9);

void updateScreen()
{
  time_counter = millis();
  DynamicJsonDocument *schedule = apiHandler->getTeamScheduleToday(teamId);

  if (schedule == nullptr)
  {
    Serial.println("Failed to get schedule for some reason");
    return;
  }

  JsonObject scheduleObject = schedule->as<JsonObject>();

  Game *game = new Game(scheduleObject["dates"][0]["games"][0].as<JsonObject>());
  gameDrawer.drawGame(game);
}

void buttonHandler(ace_button::AceButton *button, uint8_t eventType, uint8_t buttonState)
{
  switch (eventType)
  {
  case ace_button::AceButton::kEventPressed:
    Serial.println("Pressed");
    teamId++;
    gameDrawer.drawFullscreenText("Next...");
    updateScreen();
    break;
  case ace_button::AceButton::kEventReleased:
    Serial.println("Released");
    break;
  }
}

void wifiConnectionCallback(WiFiManager *wifiManager)
{
  Serial.println("Setting up AP to connect to wifi");
  gameDrawer.drawWifi();
}

void setup()
{

  Serial.begin(115200);

  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

  dma_display = Matrix::get();

  // Allocate memory and start DMA display
  if (not dma_display->begin())
    Serial.println("****** !KABOOM! I2S memory allocation failed ***********");

  dma_display->setBrightness8(32);
  dma_display->clearScreen();

  gameDrawer.drawLoading();

  pinMode(BUTTON_PIN, INPUT);

  aceButton.getButtonConfig()->setFeature(ace_button::ButtonConfig::kFeatureClick);
  aceButton.setEventHandler(buttonHandler);

  WiFiManager wm;

  bool res;
  wm.setAPCallback(wifiConnectionCallback);
  res = wm.autoConnect("MLBScoreboard");

  if (!res)
  {
    Serial.println("Failed to connect and hit timeout");
    gameDrawer.drawFullscreenText("Failed WIFI");
    sleep(30);

    ESP.restart();
  }
  else
  {
    Serial.println("Connected to wifi");
  }

  apiHandler = new ApiHandler(&http, &doc);

  time_counter = -30001;
}

void loop()
{
  aceButton.check();
  if (millis() - time_counter > (unsigned long)30000)
  {
    updateScreen();
  }
}
