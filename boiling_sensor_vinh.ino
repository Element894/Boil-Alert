#include <LiquidCrystal_I2C.h>
#include <dht.h>
#include "pitches.h"

LiquidCrystal_I2C lcd(0x27, 16, 2);
dht DHT;
const int  potPin = A0, buttonPin = 2;
int buzzer = 9, buttonState = 0;
#define DHT11_PIN 7
int target, value, seconds = 0, minutes = 10;

//function prototypes
void startLCD();
float tare(float);

void setup() {
  Serial.begin(9600);
  float ftemp;
  pinMode(buttonPin, INPUT);
  pinMode(buzzer, OUTPUT);
  startLCD();
  lcd.setCursor(0, 0);
  lcd.print("Temp = ");
  lcd.setCursor(0, 1);
  lcd.print("Push to set temp");
  tare(ftemp);
  lcd.setCursor(0,1);
  lcd.print("Temperature set!");
  delay(2000);
  lcd.print("                ");
  target = ftemp;
}

void loop() {
  delay(1500);
  value = analogRead(potPin);
  DHT.read11(DHT11_PIN);
  //buttonState = digitalRead(buttonPin);
  while(buttonState == LOW){
    buttonState = digitalRead(buttonPin);
    if(buttonState == HIGH)
      break;
  }
  float ftemp = (DHT.temperature * 9 / 5) + 32;
  lcd.setCursor(0, 0);
  lcd.print("Temp = ");
  lcd.print(ftemp, 1);
  lcd.print("\337F");
  lcd.setCursor(0, 1);
  lcd.print("                 ");
  lcd.setCursor(0, 1);
  lcd.print("Time: ");
  lcd.setCursor(6,1);
  minutes -= 1;
  lcd.print(minutes);
  lcd.setCursor(7,1);
  lcd.print('m');
  while(minutes != 0){
  seconds = 59;
  for(int i = 0; i < 60; i++){
    lcd.setCursor(11,1);
    lcd.print(seconds);
    lcd.setCursor(13,1);
    lcd.print('s');
    seconds--;
    delay(1000);
    if(seconds == 0);
      seconds == 59;
    }
    lcd.setCursor(6,1);
    lcd.print(minutes);
    lcd.setCursor(7,1);
    lcd.print('m');
    minutes--;
  }
  delay(1500);
}

//functions
void startLCD() {
  lcd.begin();
  lcd.setCursor(0, 0);
  lcd.print("   Starting");
  lcd.setCursor(0, 1);
  lcd.print("  Sensor....");
  delay(1000);
  lcd.clear();
}

float tare(float ftemp) {
  //buttonState = digitalRead(buttonPin);
  while(buttonState == LOW){ //TARE function
    buttonState = digitalRead(buttonPin);
    DHT.read11(DHT11_PIN);
    ftemp = (DHT.temperature * 9 / 5) + 32;
    if (buttonState == HIGH)
      break;
    else{
    lcd.setCursor(7, 0);
    lcd.print(ftemp, 1);
    lcd.print("\337F");
    Serial.print("Tare loop\n");
    delay(1500);
    }
  }
  Serial.print(ftemp);
  return ftemp;
}
