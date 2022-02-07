//#include <Wire.h>
//#include<LiquidCrystal_I2C.h>
//#include <ESP8266WiFi.h>
//#include <ESP8266Ping.h>
//#include <ArduinoJson.h>
//
//char* commands[2];
//const char* ssid = "myNet2.4";
//const char* password = "AndrewGolightly";
//long timer;
//LiquidCrystal_I2C lcd(0x3f, 20, 4);
//WiFiClient client;
//
//char* encryptDecrypt(char *input) {
//  char key[] = {171}; //Can be any chars, and any size array
//
//  char* output;
//
//  int i;
//  for (i = 0; i < strlen(input); i++) {
//    output[i] = input[i] ^ key[i % (sizeof(key) / sizeof(char))];
//  }
//
//  return output;
//}
//
//// TODO: fill in with actual temp checking.
//bool checkTemp()
//{
//  //  long randomNumber;
//  //
//  //  randomNumber = random(300);
//  //
//  //  if (randomNumber % 2 == 0)
//  //  {
//  //    return true;
//  //  }
//  //
//  //  return false;
//
//  return true;
//}
//
//void setup() {
//  Serial.begin(9600);
//
//  lcd.begin();
//  lcd.backlight();
//
//  WiFi.begin(ssid, password);
//
//  while (WiFi.status() != WL_CONNECTED)
//  {
//    lcd.clear();
//    lcd.print("Not Connected");
//    delay(3000);
//  }
//
//  lcd.clear();
//  lcd.print("Connected");
//}
//
//void loop() {
//  // put your main code here, to run repeatedly:
//  //  if (client.connect("192.168.1.4", 9999))
//  //  {
//  //    if (checkTemp())
//  //    {
//  //      client.print(commands[0]);
//  //      Serial.println(commands[0]);
//  //    }
//  //    else
//  //    {
//  //      client.print(commands[1]);
//  //      Serial.println(commands[1]);
//  //    }
//  //
//  //    client.stop();
//  //  }
//  //  else
//  //  {
//  //    Serial.println("Unable to connect to IP");
//  //  }
//
//  //delay(5000);
//
//  if (client.connect("192.168.1.4", 9999))
//  {
//    const uint8_t etOn[46] = {0x00, 0x00, 0x00, 0x2a, 0xd0, 0xf2, 0x81, 0xf8, 0x8b, 0xff, 0x9a, 0xf7, 0xd5, 0xef, 0x94, 0xb6, 0xc5, 0xa0, 0xd4, 0x8b, 0xf9, 0x9c, 0xf0, 0x91, 0xe8, 0xb7, 0xc4, 0xb0, 0xd1, 0xa5, 0xc0, 0xe2, 0xd8, 0xa3, 0x81, 0xf2, 0x86, 0xe7, 0x93, 0xf6, 0xd4, 0xee, 0xdf, 0xa2, 0xdf, 0xa2};
//    size_t size = 46;
//    
//    client.write(etOn, size); 
//
//    delay(1000);
//
//    client.stop();
//  }
//  else
//  {
//    Serial.println("Unable to connect to IP");
//  }
//
//  delay(5000);
//
//  Serial.println();
//
//  if (client.connect("192.168.1.4", 9999))
//  {
////    delay(100);
//
//    const uint8_t etOff[46] = {0x00, 0x00, 0x00, 0x2a, 0xd0, 0xf2, 0x81, 0xf8, 0x8b, 0xff, 0x9a, 0xf7, 0xd5, 0xef, 0x94, 0xb6, 0xc5, 0xa0, 0xd4, 0x8b, 0xf9, 0x9c, 0xf0, 0x91, 0xe8, 0xb7, 0xc4, 0xb0, 0xd1, 0xa5, 0xc0, 0xe2, 0xd8, 0xa3, 0x81, 0xf2, 0x86, 0xe7, 0x93, 0xf6, 0xd4, 0xee, 0xde, 0xa3, 0xde, 0xa3};
//    size_t size = 46;
//    
////    for (int i = 0; i < 46; i++)
////    {
////      int written = 0;
////      while (written == 0)
////      {
////        written = client.write(etOff[i]);
////      }
////      Serial.printf("%02x, ", etOff[i]);
////    }
//
//    client.write(etOff, 46);
//
//    delay(1000);
//
//    client.stop();
//  }
//  else
//  {
//    Serial.println("Unable to connect to IP");
//  }
//
//  Serial.println();
//
//  delay(5000);
//}
