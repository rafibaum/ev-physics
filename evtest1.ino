#include <LiquidCrystal.h>

LiquidCrystal lcd(8,9,4,5,6,7);
int motor1 = 10;

void setup() {
  // put your setup code here, to run once:
  pinMode(motor1, OUTPUT);
}

void loop() {
  analogWrite(motor1, 128);
  delay(1000);
}
