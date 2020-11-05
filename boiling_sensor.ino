#include <LiquidCrystal_I2C.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <EEPROM.H>

// Data wire is plugged into pin 2 on the Arduino
#define ONE_WIRE_BUS 2

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);

// Reference variable for tare function
int Rtemp = 60;

int BUTTON_STATE = LOW;
static const int TARE_PIN = 3;
// Boiling true = 1
char Boiling = 0;

void setup(void)
{
  // Start serial port
  Serial.begin(9600);
  Serial.println("Beginning setup.");

  // Start up the library
  sensors.begin(); // IC Default 9 bit. If you have troubles consider upping it 12. Ups the delay giving the IC more time to process the temperature measurement
  // Tare input
  pinMode(TARE_PIN, INPUT_PULLUP);
}


void loop(void)
{ 

  // call sensors.requestTemperatures() to issue a global temperature 
  // request to all devices on the bus
  sensors.requestTemperatures(); // Send the command to get temperatures

  // Check if temperature is at reference point
  if(sensors.getTempCByIndex(0)<Rtemp+2||sensors.getTempCByIndex(0)>Rtemp-2){
    Boiling = 1;
  };
  // Check if button is pressed, update with requirement to be held later.
  BUTTON_STATE = digitalRead(TARE_PIN);
  if(BUTTON_STATE == HIGH) {
    
  }
  Serial.print("Temperature for Device 1 is: ");
  Serial.print(sensors.getTempCByIndex(0)); // Why "byIndex"? You can have more than one IC on the same bus. 0 refers to the first IC on the wire
  
}
