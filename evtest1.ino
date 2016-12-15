#include <LiquidCrystal.h>

LiquidCrystal lcd(8,9,4,5,6,7);
const byte motor1 = 11;
const byte encPort = 2;
volatile unsigned long ticks = 0;

const long distance = 3000-125;

void setup() {
  // put your setup code here, to run once:
  pinMode(motor1, OUTPUT);
  pinMode(encPort, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(encPort), tick, CHANGE);
  Serial.begin(9600);
}

void loop() {
  if(ticks < distance) {
    analogWrite(motor1, 255);
  } else {
    analogWrite(motor1, 0);
  }
  Serial.print("ticks="+String(ticks));
  Serial.println(",button="+String(getButton()));
  delay(100);
}

void tick() {
  ticks++;
}

int getButton()
{

  static int NUM_KEYS=5;
  static int adc_key_val[5] ={  
    30, 150, 360, 535, 760     };
  int k, input;
  input=analogRead(0);
  for (k = 0; k < NUM_KEYS; k++)
  {
    if (input < adc_key_val[k])
    {
      return k;
    }
  }
  if (k >= NUM_KEYS)
    k = -1;     // No valid key pressed

  return k;
}

