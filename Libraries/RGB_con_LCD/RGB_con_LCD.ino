#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "Adafruit_TCS34725.h"

LiquidCrystal_I2C lcd(0x3F, 16, 2);

void setup(){
  Wire.begin();
  lcd.begin(16,2);
  lcd.clear();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Que pedo");
  lcd.setCursor(0,1);
  lcd.print("Como andan raza?");
}

void loop() {
}
