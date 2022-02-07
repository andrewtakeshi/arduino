#include <Adafruit_NeoPixel.h>
#define PIN 6

Adafruit_NeoPixel pix = Adafruit_NeoPixel(512, PIN);
const int _delay = 10;

void setup()
{
    pix.begin();
    pix.setBrightness(40);
}

void loop()
{
    for (int i = 0; i < 512; i++)
    {
        pix.setPixelColor(i, pix.Color(random(256), random(256), random(256)));
        pix.show();
        delay(_delay);
    }
    for (int i = 511; i >= 0; i--)
    {
        pix.setPixelColor(i, 0);
        pix.show();
        delay(_delay);
    }
}