#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>

const int pin = 6;
uint8_t rpm_signal = 0;
Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(16, 16, pin,
                                               NEO_MATRIX_LEFT + NEO_MATRIX_TOP + NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG,
                                               NEO_GRB + NEO_KHZ800);

const uint16_t RED = matrix.Color(255, 0, 0);
const uint16_t BLUE = matrix.Color(0, 0, 255);

const float rpm_shift_target = 0.85f;
const uint8_t threshold = (int)(rpm_shift_target * 256);

uint16_t color = 0;

void setup()
{
    Serial.begin(9600);
    matrix.begin();
    matrix.setBrightness(10);
    matrix.fillScreen(0);
    matrix.show();
}

void loop()
{
    // matrix.setBrightness(rpm_signal);
    matrix.fillScreen(color);
    matrix.show();
}

void serialEvent()
{
    while (Serial.available())
    {
        int temp = Serial.read();
        if (temp == -1)
        {
            color = BLUE;
            return;
        }
        rpm_signal = (uint8_t)temp;
        // color = rpm_signal > threshold ? RED : matrix.Color(rpm_signal, 255 - rpm_signal, 0);
        color = matrix.Color(rpm_signal, 255 - rpm_signal, 0);
    }
}
