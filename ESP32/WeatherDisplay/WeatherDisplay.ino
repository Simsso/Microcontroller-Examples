/*
    Downloads the five day weather forecast from openweathermap.org and
    shows it on an OLED display.
    The code is still under development and not fully functional yet.
*/

#include "OpenWeatherMapAPI.h"
#include "WeatherForecastSample.cpp"
#include "OLEDScreen.h"

const char *ssid = "SSID";
const char *password = "Password";

void setup()
{
  Serial.begin(115200);

  OpenWeatherMapAPI::init(ssid, password);
  OLEDScreen::init();
  update();
}

void loop() {

}

void update() {
  int count = 0;
  WeatherForecastSample* data;
  OpenWeatherMapAPI::getForecast("Karlsruhe,de", "API_KEY", &data, &count);
  delay(500);
  OLEDScreen::printForecast(data, count);
}

