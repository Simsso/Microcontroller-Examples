#include <Arduino.h>
#include <WiFi.h>
#include <ArduinoJson.h>

#include "OpenWeatherMapAPI.h"


const char *owmApiHost = "api.openweathermap.org";

const uint64_t timeout = 5000; // [ms]


DynamicJsonBuffer jsonBuffer(20000); // response body size is usually around 15 kByte

void OpenWeatherMapAPI::init(const char* ssid, const char* password)
{
  Serial.println("Connecting to " + String(ssid));
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) { }
  Serial.println("Connected with IP address " + WiFi.localIP().toString());
};

bool OpenWeatherMapAPI::getForecast(String query, String appId, WeatherForecastSample** forecast, int* count)
{
	const JsonObject* root = OpenWeatherMapAPI::getData(query, appId);
  if (root == nullptr) return false;
  
	*count = 20;
	*forecast = new WeatherForecastSample[*count];
	for (int i = 0; i < *count; i++) {
		(*forecast)[i].dt = (*root)["list"][i]["dt"].as<unsigned long>();
		(*forecast)[i].temp = (*root)["list"][i]["main"]["temp"].as<float>();
		(*forecast)[i].description = (*root)["list"][i]["weather"][0]["description"].as<String>();
		(*forecast)[i].dtTxt = (*root)["list"][i]["dt_txt"].as<String>();
		if ((*forecast)[i].dtTxt.length() == 0) {
			*count = i;
			break;
		}
	}
  Serial.println("Forecast data parsed");
	return true;
};

String OpenWeatherMapAPI::getResponse(String query, String appId)
{
	WiFiClient client;
	const int httpPort = 80;
	if (!client.connect(owmApiHost, httpPort)) {
    	Serial.println("Connection failed");
    	return "";
	}

	String path = "/data/2.5/forecast?q=" + query + "&appid=" + appId;
	client.print(
		"GET " + path + " HTTP/1.1\r\n" + 
		"Host: " + owmApiHost + "\r\n" + 
		"Connection: close\r\n" + 
		"Pragma: no-cache\r\n" + 
		"Cache-Control: no-cache\r\n" + 
		"User-Agent: ESP32\r\n" + 
		"Accept: text/html,application/json\r\n\r\n");
    
	uint64_t startMillis = millis();
	while (client.available() == 0) {
		if (millis() - startMillis > timeout) {
			Serial.println("Client timeout");
			client.stop();
			return "";
		}
	}

	String resHeader = "", resBody = "";
	bool receivingHeader = true;
	while(client.available()) {
		String line = client.readStringUntil('\r');
		if (line.length() == 1 && resBody.length() == 0) {
			receivingHeader = false;
			continue;
		}
		if (receivingHeader) {
			resHeader += line;
		}
		else {
			resBody += line;
		}
	}

	client.stop();

	Serial.print("Data received successfully (request body size: " + String(resBody.length()) + " bytes)");
  Serial.println(resBody);
	return resBody;
};

JsonObject* OpenWeatherMapAPI::getData(String query, String appId)
{
  String resBody = OpenWeatherMapAPI::getResponse(query, appId);
	const int contentLength = resBody.length();
	if (contentLength > 0) {
		// parse JSON
		JsonObject& root = jsonBuffer.parseObject(resBody);
		if (!root.success()) {
			return nullptr;
		}
		else {
			return &root;
		}
	}
	return nullptr;
};
