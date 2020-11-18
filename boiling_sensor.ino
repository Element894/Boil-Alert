#include <LiquidCrystal_I2C.h>
#include <LiquidCrystal.h>
#include <OneWire.h> // Change to OneWire2.h, change is drop-in replacement
#include <DallasTemperature.h>
#include <EEPROM.h>


// Declare lcd into the global namespace
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Temperature sensor pin is set to 2
#define ONE_WIRE_BUS 2

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);

//Note: Update all names to a single format I.E. avoid mixing all caps and camel case

int buzzerPin = 5, Rtemp = 60;
static const int tarePin = 7;
// Boiling true = 1
char Boiling = 0;

// Tare button timing variables
//unsigned long interval = 3000;
//unsigned long previousMillis = 0;

void setup(void)
{
  // Set pin input/output
  pinMode(tarePin, INPUT_PULLUP);
  pinMode(buzzerPin, OUTPUT);
  
  // Check reference temperature
  if (EEPROM.read(0) == 0) {
  int Rtemp = 60;
  } else {
  int Rtemp = EEPROM.read(0);
  Serial.print("Read temperature from EEPROM: ");
  Serial.println(Rtemp);
};
  // Start serial port
  Serial.begin(9600);
  Serial.println("Serial output initialized.");

  // Start up the library
  sensors.begin(); // IC Default 9 bit. If you have troubles consider upping it 12. Ups the delay giving the IC more time to process the temperature measurement
  // Tare input
  pinMode(tarePin, INPUT_PULLUP);
}

int tareAverage(void) {
  // Number of times to average
  int AvgNum = 5, Sum = 0;
  float Average = 0, Temperatures [AvgNum] = {};
  
  for (char Tindex = 0; Tindex < AvgNum; Tindex++) {
    sensors.requestTemperatures();
    Temperatures[Tindex] = sensors.getTempCByIndex(0);
    delay(1000); // Replace with Millis code to keep cpu usage while the function waits
  };
  for (int a = 0; a < AvgNum; a++) {
    Sum += Temperatures[a];
  };
  // Return the average as an integer so that the EEPROM can accept it
  Serial.print("Tare temperature set to: ");
  Serial.println(static_cast<int>(Sum/AvgNum));
  return static_cast<int>(Sum/AvgNum);
};

// Passes the reference temperature to caller, created to avoid global scope variable.
int Rtempget(void) {
   // Check reference temperature
  if (EEPROM.read(0) == 0) {
    int Rtemp = 60;
    Serial.println("No value read from EEPROM, setting temperature to 60.");
    return Rtemp;
    } else {
  int Rtemp = EEPROM.read(0);
  Serial.print("Value read from EEPROM is: ");
  Serial.println(Rtemp);
  return Rtemp;
  };
};

// Buzzer function
void buzzerOutput(void) {
  
};

void loop(void)
{ 
  // Set reference temperature within loop scope.
  int Rtemp = Rtempget();
  // call sensors.requestTemperatures() to issue a global temperature 
  // request to all devices on the bus
  sensors.requestTemperatures(); // Send the command to get temperatures

  // Check if temperature is within 2 degrees of reference value.
  if(sensors.getTempCByIndex(0)<Rtemp+2||sensors.getTempCByIndex(0)>Rtemp-2){
    Boiling = 1;
    Serial.println("Lid temperature has reached reference temperature.");
    Serial.print("Temperature is: ");
    Serial.println(sensors.getTempCByIndex(0));
    Serial.print("Reference point is: ");
    Serial.println(Rtemp);
    lcd.setCursor(0,0);
    lcd.print("Pot is boiling.");
  };
  if(sensors.getTempCByIndex(0)<Rtemp) {
    Serial.println("Lid temperature below reference temperature.");
  };
  
  
//  unsigned long currentMillis = millis();
  // Check if button is pressed for at least 3 seconds.
//  while(digitalRead(tarePin) == LOW && currentMillis - previousMillis < interval) {
//    previousMillis = currentMillis;
//    unsigned long currentMillis = millis();
//  };

  // Concept: play buzzer if temperature is >= 100
 
  // Check if button is pressed, update with requirement to be held later.
  // digital read value should be LOW because of the pull up resistor. Erroneous values are being pulled because a pull up resistor is needed on the circuit.
  if(digitalRead(tarePin) == LOW) {
    // Write the temperature to register 0
    Serial.println("Received button input.");
    Serial.println(digitalRead(tarePin));
    EEPROM.write(0, tareAverage());
    lcd.setCursor(0,0);
    lcd.print("Tared to ");
    lcd.print(sensors.getTempCByIndex(0));
  };
  Serial.print("Temperature for Device 1 is: ");
  Serial.println(sensors.getTempCByIndex(0)); // Why "byIndex"? You can have more than one IC on the same bus. 0 refers to the first IC on the wire 
};
