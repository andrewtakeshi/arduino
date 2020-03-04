#include <LiquidCrystal.h>

// Set the LCD address to 0x27 for a 20 chars and 4 line display
LiquidCrystal lcd(0x27, 20, 4);

void setup()
{
  // initialize the LCD
  lcd.begin();

  // Turn on the blacklight and print a message.
  lcd.backlight();
  lcd.print("Hello, world!");
}

void loop()
{
  // Do nothing here...
}
