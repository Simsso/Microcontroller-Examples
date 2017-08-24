#ifndef OpenWeatherMapAPI_h
#define OpenWeatherMapAPI_h

#include <ArduinoJson.h>
#include "OLEDScreen.cpp"

class OpenWeatherMapAPI
{
public:
	static void init(const char* ssid, const char* password);
	static bool getForecast(
		String query, 
		String appId, 
		WeatherForecastSample** forecast, 
		int* count);

private:
	static JsonObject* getData(String query, String appId);

	static String getResponse(String query, String appId);
};

#endif