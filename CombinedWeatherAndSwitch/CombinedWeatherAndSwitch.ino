#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <ESP8266WiFi.h>
#include <ESP8266Ping.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>
#include <ArduinoJson.h>

char* commands[2];

const char* ssid = "myNet2.4";
const char* password = "AndrewGolightly";

const char* host = "api.openweathermap.org";
String apiKey = "522a8f91cac94a8f332175041050035a";
const char* fingerprint = "6c 9d 1e 27 f1 13 7b c7 b6 15 90 13 f2 d0 29 97 a4 5b 3f 7e";

LiquidCrystal_I2C lcd(0x3f, 20, 4);

WiFiClient client;
WiFiClientSecure clientS;

Adafruit_BMP280 bme;

typedef struct
{
  String t_cur;
  String t_min;
  String t_max;
  String desc;
} result_t;

result_t getWeather(String zip)
{
  String body = "";
  bool finishedHeaders = false;
  bool currentLineBlank = true;
  bool gotResponse = false;
  long now;
  result_t ret = { "", "", "" };

  clientS.setFingerprint(fingerprint);

  if (clientS.connect(host, 443))
  {
    // Make the request - imperial units
    String URL = "/data/2.5/weather?zip=" + zip + ",us&appid=" + apiKey + "&units=imperial";

    clientS.println("GET " + URL + " HTTP/1.1");
    clientS.print("Host: ");
    clientS.println(host);
    clientS.println("User-Agent: arduino/1.0");
    clientS.println("Connection: close");
    clientS.println();

    now = millis();

    while (millis() - now < 1500) {
      while (clientS.available()) {
        char c = clientS.read();

        if (finishedHeaders)
        {
          body = body + c;
        }
        else
        {
          if (currentLineBlank && c == '\n')
          {
            finishedHeaders = true;
          }
        }

        if (c == '\n')
        {
          currentLineBlank = true;
        }
        else if (c != '\r')
        {
          currentLineBlank = false;
        }

        gotResponse = true;
      }
    }

    clientS.stop();

    if (gotResponse)
    {
      DynamicJsonDocument jsonBuffer(16384);

      DeserializationError err = deserializeJson(jsonBuffer, body);

      if(err)
      {
        Serial.print("Deserialization Error: ");
        Serial.println(err.c_str());
        return ret;
      }
      
      JsonObject current = jsonBuffer.as<JsonObject>()["main"];
      JsonObject weather = jsonBuffer.as<JsonObject>()["weather"][0];

      if (current.containsKey("temp"))
      {
        ret.t_cur = current["temp"].as<String>();
      }
      if (current.containsKey("temp_min"))
      {
        ret.t_min = current["temp_min"].as<String>();
      }
      if (current.containsKey("temp_max"))
      {
        ret.t_max = current["temp_max"].as<String>();
      }

      if (weather.containsKey("main"))
      {
        ret.desc = weather["main"].as<String>();
      }
    }
  }
  else
  {
    Serial.println("Couldn't connect to host");
  }

  return ret;
}

void turnOn()
{
  if (client.connect("192.168.1.3", 9999))
  {
    const uint8_t etOn[46] = {0x00, 0x00, 0x00, 0x2a, 0xd0, 0xf2, 0x81, 0xf8, 0x8b, 0xff, 0x9a, 0xf7, 0xd5, 0xef, 0x94, 0xb6, 0xc5, 0xa0, 0xd4, 0x8b, 0xf9, 0x9c, 0xf0, 0x91, 0xe8, 0xb7, 0xc4, 0xb0, 0xd1, 0xa5, 0xc0, 0xe2, 0xd8, 0xa3, 0x81, 0xf2, 0x86, 0xe7, 0x93, 0xf6, 0xd4, 0xee, 0xdf, 0xa2, 0xdf, 0xa2};
    size_t size = 46;

    client.write(etOn, size);
    delay(1000);
    client.stop();
  }
  else
  {
    Serial.println("Unable to connect to IP");
  }
}

void turnOff()
{
  if (client.connect("192.168.1.3", 9999))
  {
    const uint8_t etOff[46] = {0x00, 0x00, 0x00, 0x2a, 0xd0, 0xf2, 0x81, 0xf8, 0x8b, 0xff, 0x9a, 0xf7, 0xd5, 0xef, 0x94, 0xb6, 0xc5, 0xa0, 0xd4, 0x8b, 0xf9, 0x9c, 0xf0, 0x91, 0xe8, 0xb7, 0xc4, 0xb0, 0xd1, 0xa5, 0xc0, 0xe2, 0xd8, 0xa3, 0x81, 0xf2, 0x86, 0xe7, 0x93, 0xf6, 0xd4, 0xee, 0xde, 0xa3, 0xde, 0xa3};
    size_t size = 46;

    client.write(etOff, 46);
    delay(1000);
    client.stop();
  }
  else
  {
    Serial.println("Unable to connect to IP");
  }
}

int checkTemp()
{
  int tempC = bme.readTemperature();
  int tempF = tempC * (9/5) + 32;

  Serial.print("Current temperature: ");
  Serial.println(tempF);

  return tempF;
}

void setup() {
}

void loop() {
  if(checkTemp() > 70)
  {
    Serial.println("Turning on.");
    turnOn(); 
  }
  else
  {
    Serial.println("Turning off.");
    turnOff();
  }

  delay(60000);
}
