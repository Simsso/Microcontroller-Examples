#include <WiFi.h>

const char* ssid = "Access Point Name";
const char* password = "Password";

WiFiServer server(80);

String host;

void setup()
{
    Serial.begin(115200);

    Serial.println("Connecting to" + String(ssid));
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {}
    host = WiFi.localIP().toString();
    Serial.println("Connected with IP address " + host);
    Serial.println("Visit http://" + host + "/ in your browser");
    
    server.begin();
}

void loop(){
  WiFiClient client = server.available(); // listen for incoming clients

  if (client) {
    String currRequestLine = ""; // buffer for the incoming request
    int lineCount = 0;
    String firstLine;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        if (c == '\n') {
          lineCount++;
         
          // read request header (first line)
          if (lineCount == 1) {
            firstLine = currRequestLine;
            Serial.println(firstLine);
          }
          
          if (currRequestLine.length() == 0) {
            // request end
            sendResponse(firstLine, client);
            break;
          } else {
            currRequestLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currRequestLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // close the connection:
    client.stop();
  }
}

void sendResponse(String reqFirstLine, WiFiClient client) {
  if (reqFirstLine == "GET / HTTP/1.1") {
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println();

    // send minified index.html file
    client.print("<!DOCTYPE html><html><head><title>ESP32 Sensor</title><style type=\"text/css\">.box {width: 100px;height: 100px;margin: 0 auto;padding: 2.5px;position: relative;box-shadow: 0 1px 3px rgba(0,0,0,.12), 0 1px 2px rgba(0,0,0,.24);}.point {width: 5px;height: 5px;margin-left: -2.5px;margin-top: -2.5px;background: red;border-radius: 50%;position: relative;}</style></head><body><main><div class=\"box\"><div class=\"point\"></div></div><div class=\"time\"></div></main><script src=\"https://code.jquery.com/jquery-3.2.1.min.js\" integritysha256-hwg4gsxgFZhOsEEamdOYGBf13FyQuiTwlAQgxVSNgt4=\" crossorigin=\"anonymous\"></script><script type=\"text/javascript\">(function(jQuery) {let box = jQuery('.box'), time = jQuery('.time'), point = jQuery('.point');function updatePoint() {jQuery.ajax({url: 'http://"+host+"/sensor',type: 'GET',success: (res) => {point.css({ 'left': res.x * box.width(), 'top': res.y * box.height() });time.html(res.time);setTimeout(updatePoint, 10);},error: () => {setTimeout(updatePoint, 1000);}})}updatePoint();})(jQuery);</script></body></html>");
  }
  else if (reqFirstLine == "GET /sensor HTTP/1.1") {
    // response header
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: application/json");
    client.println();

    // send sensor data in JSON format
    client.print("{ \"x\": " + String(analogRead(35) / 4096.0) + ", \"y\": " + String(analogRead(34) / 4096.0) + ", \"time\": " + millis() + " }");
  }
  else {
    // 404 not found
    client.println("HTTP/1.1 404 Not Found");
    client.println("Content-Type: text/html");
    client.println();
    client.print("This page is not available.");
  }

  client.println();
}

