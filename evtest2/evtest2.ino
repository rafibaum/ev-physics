#include <LiquidCrystal.h>

LiquidCrystal lcd(8,9,4,5,6,7);
const byte rMotor = 44;
const byte lMotor = 45;
const byte lEncPort = 18,rEncPort=19;
volatile unsigned long lTicks = 0,rTicks=0;

long distance = 0;
int index = 0;

bool exitSetup = false;

void setup() {
  // put your setup code here, to run once:
  pinMode(lMotor, OUTPUT);
  pinMode(rMotor, OUTPUT);
  pinMode(lEncPort, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(lEncPort), lTick, CHANGE);
  
  pinMode(rEncPort, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(rEncPort), rTick, CHANGE);
  Serial.begin(9600);
  lcd.begin(16,2);
  
  while(!exitSetup) {
    int buttonVal=getButton();
    switch(buttonVal) {
      case 0:
        index--;
        break;
      case 1:
        distance += pow(10, index);
        break;
      case 2:
        distance -= pow(10, index);
        break;
      case 3:
        index++;
        break;
      case 4:
        exitSetup = true;
        break;
    }
    index = constrain(index, 0, 5);
    distance = constrain(distance, 0, 99999);
    //Serial.println("Index: " + String(index));
    //Serial.println("Distance: " + String(distance));
    printNum(index, distance);
    while(buttonVal>-1&&getButton() > -1) {
      Serial.println("Button: " + String(getButton()));
      delay(20);
    }
    delay(50);
  }

  lTicks=0;
  rTicks=0;
  //DISTANCE CALCULATION HERE
  distance = (long)(15.675*1*(distance/2.54)-611.79*1);
  //printNum(index, distance);
}

void loop() {
  if(lTicks < distance) {
    analogWrite(lMotor, constrain(255+(rTicks-lTicks)*10,0,255));
    analogWrite(rMotor, constrain(255+(lTicks-rTicks)*10,0,255));
  } else {
    analogWrite(lMotor, 0);
    analogWrite(rMotor, 0);
  }
  //Serial.print("ticks="+String(ticks));
  //Serial.println(",button="+String(getButton()));
  //delay(100);
}

void lTick() {
  lTicks++;
}
void rTick()
{
  rTicks++;
}

int getButton()
{

  static int NUM_KEYS=5;
  static int adc_key_val[5] ={  
    30, 150, 360, 535, 760     };
  int input;
  int k;
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

void printNum(int a, int b) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(pad(b));
  lcd.setCursor(4-a, 1);
  lcd.print("^");
}
String pad( int num ) {
 int currentMax = 10;
 String res="";
 for (byte i=1; i<5; i++){
   if (num < currentMax) {
     res+="0";
   }
   currentMax *= 10;
 }
 return res+String(num);
}
