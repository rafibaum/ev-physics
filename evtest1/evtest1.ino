#include <LiquidCrystal.h>

LiquidCrystal lcd(8,9,4,5,6,7);
const byte motor1 = 44;
const byte motor2 = 45;
const byte encPort = 18;
const byte encPort2 = 19;
volatile unsigned long ticks = 0;
volatile unsigned long ticks2 = 0;

long distance = 0;
int index = 0;

bool exitSetup = false;

const float circum = 3.141593*9*2.54*15.675;

int state = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(motor1, OUTPUT);
  pinMode(motor2, OUTPUT);
  pinMode(encPort, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(encPort), tick, CHANGE);
  attachInterrupt(digitalPinToInterrupt(encPort2), tick2, CHANGE);
  Serial.begin(9600);
  lcd.begin(16,2);

  pinMode (10,OUTPUT);
  digitalWrite(10,HIGH);
  printNum(index,distance);
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
    //printNum(index, distance);
    if(buttonVal>-1)
    printNum(index,distance);
    while(buttonVal>-1&&getButton() > -1) {
      //Serial.println("Button: " + String(getButton()));
      delay(20);
    }
    delay(50);
  }

  ticks=0;
  ticks2=0;
  //DISTANCE CALCULATION HERE
  distance=(long)(5.4374*distance-71.042);
  //printNum(index, distance);
  lcd.setCursor(0,1);
  lcd.print("DEG: "+String(distance));
  digitalWrite(10,LOW);
}

void loop() {
  switch(state) {
    case 0:
      drive(distance);
    break;
  }
}


void drive(long dist) {
  if((ticks+ticks2)/2 > dist) {
    analogWrite(motor1, 0);
    analogWrite(motor2, 0);
    state++;
    ticks = 0;
    ticks2 = 0;
    digitalWrite(10,HIGH);
    lcd.setCursor(12, 1);
    lcd.print("DONE");
  } else {
    analogWrite(motor1, 255);
    analogWrite(motor2, 255);
  }
}

void turn(short deg) {
  unsigned long dist = (deg/360.0)*circum;
  if((ticks+ticks2)/2 > fabs(dist)) {
    analogWrite(motor1, 0);
    analogWrite(motor2, 0);
    state++;
    ticks = 0;
    ticks2 = 0;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("DONE");
  } else if(deg > 0) {
    analogWrite(motor1, 255);
    analogWrite(motor2, 0);
  } else {
    analogWrite(motor1, 0);
    analogWrite(motor2, 255);
  }
  //Serial.print("ticks="+String(ticks));
  //Serial.println(",button="+String(getButton()));
  //delay(100);
}

void tick() {
  ticks++;
}

void tick2() {
  ticks2++;
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
