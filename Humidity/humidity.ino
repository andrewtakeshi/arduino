#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

Adafruit_BME280 bme;
AsyncWebServer server(80);
WiFiClient client;

const char *ssid = "myNet2.4";
const char *password = "AndrewGolightly";

String ender_ip = "192.168.1.110";
String light_ip = "192.168.1.100";
String room_ip = "192.168.1.102";

bool enderState = false, lightState = false, roomState = false, autoState = true;
bool enderStateOld = false, lightStateOld = false, roomStateOld = false;

const size_t _info_size = 33;
const size_t _toggle_size = 46;
const uint8_t _info[_info_size] = {0x00, 0x00, 0x00, 0x1d, 0xd0, 0xf2, 0x81, 0xf8, 0x8b, 0xff, 0x9a, 0xf7, 0xd5, 0xef, 0x94, 0xb6, 0xd1, 0xb4, 0xc0, 0x9f, 0xec, 0x95, 0xe6, 0x8f, 0xe1, 0x87, 0xe8, 0xca, 0xf0, 0x8b, 0xf6, 0x8b, 0xf6};
const uint8_t _on[_toggle_size] = {0x00, 0x00, 0x00, 0x2a, 0xd0, 0xf2, 0x81, 0xf8, 0x8b, 0xff, 0x9a, 0xf7, 0xd5, 0xef, 0x94, 0xb6, 0xc5, 0xa0, 0xd4, 0x8b, 0xf9, 0x9c, 0xf0, 0x91, 0xe8, 0xb7, 0xc4, 0xb0, 0xd1, 0xa5, 0xc0, 0xe2, 0xd8, 0xa3, 0x81, 0xf2, 0x86, 0xe7, 0x93, 0xf6, 0xd4, 0xee, 0xdf, 0xa2, 0xdf, 0xa2};
const uint8_t _off[_toggle_size] = {0x00, 0x00, 0x00, 0x2a, 0xd0, 0xf2, 0x81, 0xf8, 0x8b, 0xff, 0x9a, 0xf7, 0xd5, 0xef, 0x94, 0xb6, 0xc5, 0xa0, 0xd4, 0x8b, 0xf9, 0x9c, 0xf0, 0x91, 0xe8, 0xb7, 0xc4, 0xb0, 0xd1, 0xa5, 0xc0, 0xe2, 0xd8, 0xa3, 0x81, 0xf2, 0x86, 0xe7, 0x93, 0xf6, 0xd4, 0xee, 0xde, 0xa3, 0xde, 0xa3};

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
<html>
<head>
    <title>ESP Web Server</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <meta http-equiv="Refresh" content="10">
    <style>
        html {
            font-family: monospace;
            display: inline-block;
            text-align: center;
        }

        h2 {
            font-size: 3.0rem;
        }

        p {
            font-size: 3.0rem;
        }

        body {
            max-width: 600px;
            margin: 0px auto;
            padding-bottom: 25px;
        }


        input[type="checkbox"].switch_2 {
            -webkit-appearance: none;
            -moz-appearance: none;
            appearance: none;
            width: 100px;
            height: 8px;
            background: #444;
            border-radius: 5px;
            position: relative;
            outline: 0;
            cursor: pointer;
        }

        input[type="checkbox"].switch_2:before,
        input[type="checkbox"].switch_2:after {
            position: absolute;
            content: "";
            -webkit-transition: all .25s;
            transition: all .25s;
        }

        input[type="checkbox"].switch_2:before {
            width: 40px;
            height: 40px;
            background: #ccc;
            border: 5px solid #666;
            border-radius: 50%%;
            top: 50%%;
            left: 0;
            -webkit-transform: translateY(-50%%);
            transform: translateY(-50%%);
        }

        input[type="checkbox"].switch_2:after {
            width: 30px;
            height: 30px;
            background: #666;
            border-radius: 50%%;
            top: 50%%;
            left: 10px;
            -webkit-transform: scale(1) translateY(-50%%);
            transform: scale(1) translateY(-50%%);
            -webkit-transform-origin: 50%% 50%%;
            transform-origin: 50%% 50%%;
        }

        input[type="checkbox"].switch_2:checked:before {
            left: calc(100%% - 35px);
        }

        input[type="checkbox"].switch_2:checked:after {
            left: 75px;
            -webkit-transform: scale(0);
            transform: scale(0);
        }
    </style>
</head>
<body>

<h2><label for="enderCheck">Ender</label></h2>
<input type="checkbox" onchange="toggleCheckbox(this)" id="enderCheck" class="switch_2">
<h2><label for="lightCheck">Light</label></h2>
<input type="checkbox" onchange="toggleCheckbox(this)" id="lightCheck" class="switch_2">
<h2><label for="roomCheck">Room</label></h2>
<input type="checkbox" onchange="toggleCheckbox(this)" id="roomCheck" class="switch_2">
<h2><label for="autoToggle">Auto</label></h2>
<input type="checkbox" onchange="toggleCheckbox(this)" id="autoToggle" class="switch_2">
<div style="width: 100%%; height: 50px;"></div>

%BMESTATS%

%STATESCRIPT%

<script>function toggleCheckbox(element) {
    let xhr = new XMLHttpRequest();
    let endpoint="/update?";
    if (element.id === "enderCheck") {
        endpoint += "ender";
    } else if (element.id === "lightCheck") {
        endpoint += "light";
    } else if (element.id === "roomCheck") {
        console.log("getting room endpoint");
        endpoint +="room";
    } else {
        endpoint += "autoToggle";
    }
    console.log(endpoint);
    xhr.open("GET", endpoint, true);
    xhr.send();
}
</script>
</body>
</html>
)rawliteral";

enum switch_t
{
  ENDER = 0,
  LIGHT,
  ROOM,
  AUTO
};

int tempF()
{
  float tempC = bme.readTemperature();
  float tempF = tempC * (9 / 5) + 32;
  return round(tempF);
}

bool checkSwitchState(switch_t _switch)
{
  String ip;
  switch (_switch)
  {
  case ENDER:
    ip = ender_ip;
    break;
  case LIGHT:
    ip = light_ip;
    break;
  case ROOM:
    ip = room_ip;
    break;
  default:
    return autoState;
  }

  if (client.connect(ip, 9999))
  {
    uint8_t key = 171;
    String payload;

    client.write(_info, _info_size);
    delay(1000);

    while (client.available() > 0)
    {
      char data = client.read();
      char decoded = key ^ data;
      key = data;
      payload += decoded;
    }

    int start_idx = payload.indexOf("relay_state", 175);

    if (!start_idx)
    {
      client.stop();
      return false;
    }

    bool state = payload.charAt(start_idx + 13) - '0';

    client.stop();

    return state;
  }
  else
  {
    return false;
  }
}

bool toggleState(switch_t _switch)
{
  String ip;
  bool _state;

  switch (_switch)
  {
  case ENDER:
    ip = ender_ip;
    _state = enderState;
    break;
  case LIGHT:
    ip = light_ip;
    _state = lightState;
    break;
  case ROOM:
    ip = room_ip;
    _state = roomState;
    break;
  default:
    return false;
  }

  if (client.connect(ip, 9999))
  {
    if (_state)
    {
      if (client.write(_on, _toggle_size))
      {
        client.stop();
        return true;
      }
      else
      {
        client.stop();
        return false;
      }
    }
    else
    {
      if (client.write(_off, _toggle_size))
      {
        client.stop();
        return true;
      }
      else
      {
        client.stop();
        return false;
      }
    }
  }
  else
  {
    return false;
  }
}

void checkStateLoop()
{
  if (enderState != enderStateOld)
  {
    if (toggleState(ENDER))
    {
      enderStateOld = enderState;
    }
    else
    {
      enderState = enderStateOld;
    }
  }
  yield();

  if (lightState != lightStateOld)
  {
    if (toggleState(LIGHT))
    {
      lightStateOld = lightState;
    }
    else
    {
      lightState = lightStateOld;
    }
  }
  yield();

  if (roomState != roomStateOld)
  {
    if (toggleState(ROOM))
    {
      roomStateOld = roomState;
    }
    else
    {
      roomState = roomStateOld;
    }
  }
  yield();
}

String processor(const String &var)
{
  String retVal;

  if (var == "BMESTATS")
  {
    int temp = tempF();
    float humidity = bme.readHumidity();
    retVal += "<h2>Temp: ";
    Serial.printf("Temp: %d; Humidity: %f", temp, humidity);
    retVal += temp;
    retVal += "</h2>";
    retVal += "<h2>Humidity: ";
    retVal += humidity;
    retVal += "</h2>";
  }

  if (var == "STATESCRIPT")
  {
    retVal += "<script>\n";
    char *names[] = {"ender", "light", "room"};
    for (int i = 0; i < 3; i++)
    {
      retVal += "\tdocument.getElementById(\"";
      retVal += names[i];
      retVal += "Check\").checked = ";
      bool state;
      switch (i)
      {
      case 0:
        state = enderState;
        break;
      case 1:
        state = lightState;
        break;
      case 2:
        state = roomState;
        break;
      default:
        state = false;
        break;
      }
      retVal += state ? "true" : "false";
      retVal += ";\n";
    }

    retVal += "\tdocument.getElementById(\"autoToggle\").checked = ";
    retVal += autoState ? "true" : "false";
    retVal += "; </script>";
  }

  Serial.print("retval: ");
  Serial.println(retVal);

  return retVal;
}

void setup()
{
  // Serial port for debugging purposes
  Serial.begin(9600);

  client.setNoDelay(true);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  // Print ESP Local IP Address
  Serial.println(WiFi.localIP());

  if (bme.begin(0x76))
  {
    Serial.println("BME initialized");
  }
  else
  {
    Serial.println("BME failed");
  }

  enderState = checkSwitchState(ENDER);
  lightState = checkSwitchState(LIGHT);
  roomState = checkSwitchState(ROOM);
  autoState = true;

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send_P(200, "text/html", index_html, processor); });

  // Send a GET request to <ESP_IP>/update?state=<inputMessage>
  server.on("/update", HTTP_GET, [](AsyncWebServerRequest *request)
            {
              if (request->hasParam("ender"))
              {
                enderState = !enderState;
              }
              else if (request->hasParam("light"))
              {
                lightState = !lightState;
              }
              else if (request->hasParam("room"))
              {
                roomState = !roomState;
              }
              else
              {
                autoState = !autoState;
              }
              request->send(200, "text/plain", "OK");
            });
  server.begin();
}

void loop()
{
  checkStateLoop();
  if (autoState)
  {
    if (bme.readHumidity() < 35)
    {
      roomState = true;
    }
    else
    {
      roomState = false;
    }
  }
  delay(1000);
}
