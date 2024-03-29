# Arduino

Contains code related to various arduino projects I'm working on. 

### Project Descriptions
- APIExample: contains a working example of a RESTful API query using an ESP8266 and the default ESP8266Wifi library. Because the API queried uses https, it also makes use of the WiFiSecure client instead of the regular WiFiClient, and all the other fun stuff that goes along with that.
- EspWeather: contains a working example of a "weather station". It queries the openweathermap API to get current weather information for the location (can be changed in the file). It then reads the data and displays it on a LCD2004. It is currently configured to read the current temp, min temp, max temp, and current weather conditions (i.e. clear, cloudy, etc.). 
  - Both EspWeather and APIExample use a browser fingerprint for the SSL stuff. This is pulled from Chrome (or any other browser). In Chrome it's obtained by visiting the site, clicking on the lock on the left hand side of the address bar => Certificate => Details => Thumbprint. There are better ways of doing it, but I'm okay with manually replacing the value whenever the cert expires so it works for me. 
  - [WifiClient Documentation](https://arduino-esp8266.readthedocs.io/en/latest/esp8266wifi/client-examples.html)
  - [WifiClientSecure Documentation](https://arduino-esp8266.readthedocs.io/en/latest/esp8266wifi/client-secure-class.html)
- I2CScanner: not my code. Scans the host device for connected I2C devices.
- SmartSwitch: contains working example of code that can toggle on/off a TP-Link HS105 smart switch. Right now it uses hard-coded byte arrays to send the on/off messages; eventually, I would like to add a working encode/decode for the autokey XOR cipher used for encoding/decoding messages sent to the TP-Link device. Eventually, I would also like to add support for a BMP280 to read the current (local) temperature and set the switch state based on that value. The end goal with this is to have a working "smart thermometer" using an ESP8266, BMP280, and possibly LCD2004 (for displaying current data) which will turn on/off an AC unit or electric heater connected to a HS105 based on the reading of the BMP280. 
  - [This repo](https://github.com/softScheck/tplink-smartplug) is the source of tplink.py and made the project possible. 
  - IP address of the HS105 is pulled from the router and will have to be updated.
- Libraries: just libraries used in various projects.
- tplink.py: cloned and modified from [here](https://github.com/softScheck/tplink-smartplug) (the repo referenced in SmartSwitch). It's a command line tool to send commands to the HS105 and other similar devices. I modified it to print out the byte values of the messages being sent so that I could copy these over and use them in the SmartSwitch code. 
