#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#define PIN 6

Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(16, 16, 2, 1, PIN,
                                               NEO_MATRIX_TOP + NEO_MATRIX_LEFT +
                                                   NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG +
                                                   NEO_TILE_TOP + NEO_TILE_LEFT +
                                                   NEO_TILE_ROWS + NEO_TILE_PROGRESSIVE);
const int btn_pin = 2;
int x_pos = matrix.width();

int brightness[] = {0, 10, 20, 40, 80, 160, 255};
int brightness_idx = 0;

void setup()
{
  // Matrix setup
  matrix.begin();
  matrix.setTextWrap(false);
  matrix.setBrightness(brightness[brightness_idx]);
  matrix.setTextColor(matrix.Color(55, 250, 100));

  // Button interrupt setup
  pinMode(btn_pin, INPUT_PULLUP);
  attachInterrupt(
      digitalPinToInterrupt(btn_pin), []()
      {
        brightness_idx++;
        if (brightness_idx > 6)
          brightness_idx = 0;
        matrix.setBrightness(brightness[brightness_idx]);
      },
      RISING);
}

void loop()
{
  int rect_width = matrix.width();
  int rect_height = matrix.height();

  for (int i = 0; i < min(matrix.width(), matrix.height()) / 2; i++)
  {
    int r = random(256);
    int g = random(256);
    int b = random(256);
    matrix.drawRect(i, i, rect_width, rect_height, matrix.Color(r, g, b));
    matrix.show();
    rect_width -= 2;
    rect_height -= 2;
    delay(500);
  }

  rect_width = matrix.width();
  rect_height = matrix.height();

  for (int i = 0; i < min(matrix.width(), matrix.height()) / 2; i++)
  {
    matrix.drawRect(i, i, rect_width, rect_height, 0);
    matrix.show();
    rect_width -= 2;
    rect_height -= 2;
    delay(500);
  }
}
