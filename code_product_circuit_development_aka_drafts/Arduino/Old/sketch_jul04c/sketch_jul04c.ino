
// Include the libraries we need
#include <SD.h>
#include <SPI.h>
#include <OneWire.h>
#include <DallasTemperature.h>
 
// Data wire is plugged into port 2 on the Arduino
#define ONE_WIRE_BUS 2
 
// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);
 
// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);
 
int NumSensors=0;
/*
 * The setup function. We only start the sensors here
 */
 File myFile;

int pinCS = 10; // Pin 10 on Arduino Uno
void setup(void)
{
  // start serial port
  Serial.begin(9600);
    pinMode(pinCS, OUTPUT);
  
  // SD Card Initialization
  if (SD.begin())
  {
    Serial.println("SD card is ready to use.");
  } else
  {
    Serial.println("SD card initialization failed");
    return;
  }
 
  // Start up the library
  sensors.begin();
  NumSensors=sensors.getDeviceCount();
  Serial.print(NumSensors);Serial.print(" sensor");
  if((NumSensors>1)&(NumSensors!=0))
    Serial.print("s");
  Serial.println(" available");
}
 
/*
 * Main function, get and show the temperature
 */
void loop(void)
{ 
  // call sensors.requestTemperatures() to issue a global temperature 
  // request to all devices on the bus
  Serial.print("Requesting temperatures...");
  sensors.requestTemperatures(); // Send the command to get temperatures
  Serial.println("DONE");
  // After we got the temperatures, we can print them here.
  for(int i=0;i<NumSensors;i++)
  {
    Serial.print("Sensor ");Serial.print(i);Serial.print(" : ");
    Serial.print(sensors.getTempCByIndex(i));Serial.print((char)176);Serial.print("C");
          myFile = SD.open("test.txt", FILE_WRITE);
  if (myFile) {  
   
    myFile.print("Sensor ");myFile.print("i");myFile.print(" : "); 
    myFile.println(sensors.getTempCByIndex(i));myFile.print((char)176);myFile.print("C"); 
    myFile.close(); // close the file
  }
  // if the file didn't open, print an error:
  else {
    Serial.println("error opening test.txt");
  }
          if(i<NumSensors-1)
      Serial.print(",");
  }
  Serial.println();
    delay(1000);
}
