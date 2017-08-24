#ifndef OLEDScreen_h
#define OLEDScreen_h

#include <Wire.h>
#include <ACROBOTIC_SSD1306.h>

#include "WeatherForecastSample.cpp"

class OLEDScreen
{
public:
	static void init() {
    Wire.begin();
    oled.init();
    oled.setFont(font5x7); // default is 8x8
    clear();
  };
	static void printForecast(WeatherForecastSample* data, int count)
	{
  	clear();
    oled.setHorizontalMode(); 
    oled.setTextXY(0,0);
    oled.putString("Weather Forecast");

    String dtTxt = "", temp = "", description = "";
    
    for (int i = 0; i < count && i < 2; ++i)
    {
      dtTxt += padRight(data[i].dtTxt);
      temp += padRight(String(data[i].temp) + " C");
      description += padRight(data[i].description);
    }
    oled.setTextXY(2, 0);
    oled.putString(dtTxt);
    Serial.println(dtTxt);
    
    oled.setTextXY(3, 0);
    oled.putString(temp);
    Serial.println(temp);
    
    oled.setTextXY(4, 0);
    oled.putString(description);
    Serial.println(description);
  
    oled.setHorizontalScrollProperties(false, 0, 255, 20);
    oled.activateScroll();
	}

  static void clear() {
    for (int y = 0; y < 8; y++) { // line
      for (int x = 0; x < 20; x++) { // column
        oled.putChar(' ');
      }
    }
  }

private:
  static String padRight(String x) {
    while (x.length() < 20) {
      x = x + " ";
    }
    return x;
  }
};

#endif
