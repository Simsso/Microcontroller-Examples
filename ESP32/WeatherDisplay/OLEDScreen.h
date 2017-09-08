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
  
	static void printForecast(WeatherForecastSample data)
	{
  	clear();
    oled.setHorizontalMode(); 
    oled.setTextXY(0,0);
    oled.putString("Weather Forecast");
    
    String dtTxt = data.dtTxt;
    String temp = String(data.temp - 273.15) + " C";
    String description = data.description;
    
    oled.setTextXY(2, 0);
    oled.putString(dtTxt);
    
    oled.setTextXY(3, 0);
    oled.putString(temp);
    
    oled.setTextXY(4, 0);
    oled.putString(description);
  
    //oled.setHorizontalScrollProperties(false, 0, 255, 20);
    //oled.activateScroll();
	}

  static void clear() {
    for (int y = 0; y < 8; y++) { // line
      for (int x = 0; x < 20; x++) { // column
        oled.setTextXY(y, x);
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
