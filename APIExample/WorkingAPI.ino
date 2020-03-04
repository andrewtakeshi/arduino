#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>

LiquidCrystal_I2C lcd(0x3f, 20, 4);

const char* ssid = "myNet2.4";
const char* password = "AndrewGolightly";
const char* host = "jsonplaceholder.typicode.com";
const char* fingerprint = "06 68 cf cf 1d 2c e8 30 5f df 33 c9 33 c3 10 01 22 13 15 1e";

WiFiClientSecure client; 

bool testAPI()
{
  String headers = "";
  String body = "";
  
  bool finishedHeaders = false;
  bool currentLineBlank = true;
  bool gotResponse = false;
  bool ret = false;
  long now;

  client.setFingerprint(fingerprint);
  
  if (client.connect(host, 443))
  {
    Serial.println("Connected.");

    String URL = "/todos/1";

    Serial.println(URL);

    client.println("GET " + URL + " HTTP/1.1");
    client.print("Host: "); 
    client.println(host);
    client.println("User-Agent: arduino/1.0");
    client.println("Connection: close");
    client.println();

    now = millis();

    while (millis() - now < 1500)
    {
      while (client.available() || client.connected()) 
      {
        char c = client.read();
        //Serial.print(c);
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
          else
          {
            headers = headers + c;
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

  Serial.print(body);

    if (gotResponse)
    {
      DynamicJsonDocument jsonBuffer(16384);
      DeserializationError err = deserializeJson(jsonBuffer, body);
      
      if (err != DeserializationError::Ok)
      {
        Serial.println(err.c_str());
        return NULL;
      }
      
      JsonObject root = jsonBuffer.as<JsonObject>();
      
      if (root.containsKey("userID"))
      {
        ret = true;
      }
    }
    else
    {
      Serial.println("No response from server");
    }
  }
  else
  {
    Serial.println("Unable to connect to host");
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
  
  testAPI();
}

// Medical Tower Location:
// Latitude: 40.7678829463127
// Longitude: -111.83180809020998
// gridpoints/SLC/101,174/forecast

void loop()
{

}
