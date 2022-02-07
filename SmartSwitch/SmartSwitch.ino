#include <Wire.h>
#include<LiquidCrystal_I2C.h>
#include <ESP8266WiFi.h>
#include <ESP8266Ping.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>
#include <ArduinoJson.h>

char* commands[2];
const char* ssid = "myNet2.4";
const char* password = "AndrewGolightly";
const char* ss_ip = "192.168.1.3";
const char* host = "worldclockapi.com/api/json/mst/now";
long timer;
//LiquidCrystal_I2C lcd(0x3f, 20, 4);
WiFiClient client;
Adafruit_BMP280 bme;

void turnOn()
{
  if (client.connect(ss_ip, 9999))
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
  if (client.connect(ss_ip, 9999))
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

bool checkTemp()
{
  float tempC = bme.readTemperature();
  float tempF = tempC * (9/5) + 32;

  Serial.print("Current temperature: ");
  Serial.println(tempF);

  return tempF < 61;
}

void setup() {
  Serial.begin(9600);

//  lcd.begin();
//  lcd.backlight();

  if (!bme.begin())
  {
    Serial.println("Couldn't find valid BMP280");
    while (1);
  }

  bme.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(3000);
  }

  //lcd.clear();
  Serial.println("Connected");
}

void loop() {
  if (checkTemp())
  {
    turnOn();
  }
  else
  {
    turnOff();
  }

  delay(15000);
}
