#ifndef WeatherForecastSample_cpp
#define WeatherForecastSample_cpp

#include "Arduino.h"

class WeatherForecastSample
{
public:
	uint32_t dt; // date (unix timestamp)
	float temp; // temperature (Kelvin)
	float tempMin; // min temperature (Kelvin)
	float tempMax; // max temperature (Kelvin)

	// pressure
	float pressure;
	float seaLevel; 
	float grndLevel;

	uint8_t humidity;

	String txtMain;
	String description;

	uint8_t clouds;

	String dtTxt; // e.g. 2017-08-22 21:00:00
};

#endif
