// #include <ESP32-HUB75-MatrixPanel-I2S-DMA.h>
#include "constants.h"
#include "WiFi.h"
#include "ApiHandler.h"
#include "Matrix.h"
#include "GameDrawer.h"
#include "./util.hpp"
#include <WiFiManager.h>

MatrixPanel_I2S_DMA* Matrix::dma_display = nullptr;
MatrixPanel_I2S_DMA* dma_display;

unsigned long time_counter = 0;
ApiHandler* apiHandler;

//Define a score struct 
struct Score {
  int home;
  int away;
};

Score currentScore;

char ssid[] = "HowAreYouDoing";
char password[] = "livingthedream";

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 3600 * -1 * 5;
const int   daylightOffset_sec = 3600;

int status = WL_IDLE_STATUS;

HTTPClient http;
DynamicJsonDocument doc(JSON_BUFFER_SIZE);
GameDrawer gameDrawer;

void setup() {
  
  Serial.begin(115200);
  
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

  dma_display = Matrix::get();

  // Allocate memory and start DMA display
  if( not dma_display->begin() )
    Serial.println("****** !KABOOM! I2S memory allocation failed ***********");

  dma_display->setBrightness8(32);
  dma_display->clearScreen();

  gameDrawer.drawLoading();

  WiFiManager wm;

  bool res;
  res = wm.autoConnect("MLBScoreboard");

  if(!res){
    Serial.println("Failed to connect and hit timeout");
    ESP.restart();
  }
  else{
    Serial.println("Connected to wifi");
  }


  Serial.println();
  Serial.println("Connected to wifi");
  apiHandler = new ApiHandler(&http, &doc);

  time_counter = -30001;
}

void loop() {
  if(millis() - time_counter > (unsigned long)30000){
    time_counter = millis();
    JsonObject schedule = apiHandler->getTeamScheduleToday(TEAM_ID::BOSTON_REDSOX);
    // JsonObject schedule = apiHandler->getSchedule("2022-07-09");

    Game* game = new Game(schedule["dates"][0]["games"][0].as<JsonObject>());
    gameDrawer.drawGame(game);
  }
}