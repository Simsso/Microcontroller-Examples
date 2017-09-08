/*
    Downloads the five day weather forecast from openweathermap.org and
    shows it on an OLED display.

    Enter your SSID, password, location query, and OpenWeatherMap API key.
*/

#include "OpenWeatherMapAPI.h"
#include "WeatherForecastSample.cpp"
#include "OLEDScreen.h"

const char *ssid = "WIFI_SSID";
const char *password = "WIFI_PASSWORD";

const String query = "Karlsruhe,de"; // location,country
const String apiKey = "API_KEY";

const uint64_t updateInterval = 1000 * 3600 * 2; // 2 hours
uint64_t lastUpdate = 0;

void setup()
{
  Serial.begin(115200);

  OpenWeatherMapAPI::init(ssid, password);
  OLEDScreen::init();
}

void loop() {
  WeatherForecastSample* data;
  int count = 0;
  bool status = OpenWeatherMapAPI::getForecast(query, apiKey, &data, &count);
  if (!status) {
    Serial.println("An error occured");
    while (true) {}
  }
  
  lastUpdate = millis();

  while (true) {
    // time to update forecast data
    if (lastUpdate + updateInterval < millis()) {
      OpenWeatherMapAPI::getForecast(query, apiKey, &data, &count);
      lastUpdate = millis();
    }

    for (int i = 0; i < count; i++) {
      OLEDScreen::printForecast(data[i]);
      delay(5000);
    }
  }
}

