#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <ESP8266WiFi.h>
#include <ESP8266Ping.h>
#include <ArduinoJson.h>

LiquidCrystal_I2C lcd(0x3f, 20, 4);

const char* ssid = "myNet2.4";
const char* password = "AndrewGolightly";
const char* host = "api.openweathermap.org";
const char* fingerprint = "6c 9d 1e 27 f1 13 7b c7 b6 15 90 13 f2 d0 29 97 a4 5b 3f 7e";

// Using the OpenWeatherMap API
String apiKey = "522a8f91cac94a8f332175041050035a";

WiFiClientSecure client;

typedef struct
{
  String t_cur;
  String t_min;
  String t_max;
} result_t;

result_t getWeather(String zip)
{
  String body = "";
  bool finishedHeaders = false;
  bool currentLineBlank = true;
  bool gotResponse = false;
  long now;
  result_t ret = { "", "", "" };

  client.setFingerprint(fingerprint);

  if (client.connect(host, 443))
  {
    // Make the request - imperial units
    String URL = "/data/2.5/weather?zip=" + zip + ",us&appid=" + apiKey + "&units=imperial";

    client.println("GET " + URL + " HTTP/1.1");
    client.print("Host: ");
    client.println(host);
    client.println("User-Agent: arduino/1.0");
    client.println("Connection: close");
    client.println();

    now = millis();

    while (millis() - now < 1500) {
      while (client.available()) {
        char c = client.read();

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

    client.stop();

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
    }
  }
  else
  {
    Serial.println("Couldn't connect to host");
  }

  return ret;
}

void setup()
{
  Serial.begin(9600);

  // put your setup code here, to run once:
  lcd.begin();
  lcd.backlight();

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    lcd.clear();
    lcd.print("Not Connected");
    delay(3000);
  }

  lcd.clear();
  lcd.print("Connected");
}

void loop()
{
  result_t result = getWeather("84112");

  Serial.println("Current: " + result.t_cur);

  if (result.t_cur != "")
  {
    lcd.clear();
    lcd.print("Current: ");
    lcd.print(result.t_cur);
    lcd.setCursor(1, 0);
    lcd.print("Min: ");
    lcd.print(result.t_min);
    lcd.setCursor(2, 0);
    lcd.print("Max: ");
    lcd.print(result.t_max);
  }
  else
  {
    lcd.clear();
    lcd.print("Error");
  }

  // Update every 15 minutes
  delay(900000);
}
