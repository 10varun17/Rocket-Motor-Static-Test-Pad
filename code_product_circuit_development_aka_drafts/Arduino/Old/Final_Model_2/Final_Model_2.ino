#include <OneWire.h>
#include <DallasTemperature.h>
#include <Wire.h>
#include "RTClib.h"
RTC_DS1307 rtc;
#include <SD.h> // SD library

//#include "SSD1306Ascii.h"
//#include "SSD1306AsciiAvrI2c.h"
//#define I2C_ADDRESS 0x3C //Address
//#define RST_PIN -1 //For OLED with no reset pin

// Data wire is plugged into digital pin 5 on the Arduino
#define ONE_WIRE_BUS 2 //The parallel wires are connected to D2

// Setup a oneWire instance to communicate with any OneWire device
OneWire oneWire(ONE_WIRE_BUS);  

// Pass oneWire reference to DallasTemperature library
DallasTemperature sensors(&oneWire);

int deviceCount = 0;

float T1, T2, T3, T4; //Variables for the 4 thermometers

//const byte FileOnOffButton = 10; //toggle switch (ON-ON) for starting and stopping the logging 

//bool WritingEnabled = false; //we switch the status of this with the buttons
//bool SwitchStatus = false; //by default, this is false that means that the switch is OFF (no logging)

unsigned long startTime;
unsigned long elapsedTime;

//SSD1306AsciiAvrI2c display;

int CS = 4; //chip select pin for the MicroSD Card Adapter, This is the CS Pin
File myfile; // file object that is used to read and write data

void setup(void)
{
  sensors.begin(); // Start up the library
  Serial.begin(9600);
  
  sensors.setResolution(10); //10 bit resolution (0.25°C step)
    
  // locate devices on the bus
  Serial.print("Locating devices...");
  Serial.print("Found ");
  deviceCount = sensors.getDeviceCount();
  Serial.print(deviceCount, DEC);
  Serial.println(" devices.");
  Serial.println("");

  //Pin for the switch pins  
  //pinMode(FileOnOffButton, INPUT);
  //for RTC
while (!Serial); // for Leonardo/Micro/Zero
Serial.begin(9600);
if (! rtc.begin()) {
Serial.println("Couldn't find RTC");
while (1);
}
if (! rtc.isrunning()) {
Serial.println("RTC is NOT running!");
// following line sets the RTC to the date & time this sketch was compiled
// rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
// This line sets the RTC with an explicit date & time, for example to set
// January 21, 2014 at 3am you would call:
//rtc.adjust(DateTime(2019, 3, 3, 20, 0, 0));
}
//rtc.adjust(DateTime(2019, 3, 3, 20, 0, 0));
rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

  //SD card module-----------------------------------------------------------------------------------
  pinMode(CS, OUTPUT); // chip select pin is set as OUTPUT
  
  if (!SD.begin(CS)) 
  { // Initialize SD card
    Serial.println("No SD card found. Reset the device after inserting an SD card."); // if return value is false, something went wrong.
  }
    Serial.println("initialization done.");
  //Starting timer for the elapsed time
  startTime = millis();
  
}


//void ReadRTC(){
//
//}

void ReadSensors()
{  
  elapsedTime = millis() - startTime;
  
  //Collect the values for each sensors    
  sensors.requestTemperatures(); //request the temperature

  //Filling up the variables
  T1 = sensors.getTempCByIndex(0);  
  T2 = sensors.getTempCByIndex(1);  
  T3 = sensors.getTempCByIndex(2);  
  T4 = sensors.getTempCByIndex(3);         
}




void PrintSerial()
{
  DateTime now = rtc.now();
Serial.print(now.year(), DEC);
Serial.print('/');
Serial.print(now.month(), DEC);
Serial.print('/');
Serial.print(now.day(), DEC);
Serial.print(" ");
Serial.print(now.hour(), DEC);
Serial.print(':');
Serial.print(now.minute(), DEC);
Serial.print(':');
Serial.print(now.second(), DEC);
Serial.println();
delay(1000);
  //Serial.print(elapsedTime); //time in ms
  Serial.print(" ");  //space
  Serial.print(T1,2); //temperature, 2 digits (i.e. 28.12)
  Serial.print(" ");  
  Serial.print(T2,2);
  Serial.print(" ");  
  Serial.print(T3,2);
  Serial.print(" ");  
  Serial.println(T4,2);
}

void WriteSD()
{
    myfile = SD.open("final.txt", FILE_WRITE); // open "final.txt" to write data; make sure that you want to write in the same file that you created in the setup()
    myfile.seek(EOF); //This goes to the End Of (the) File and _continues_ writing from there
    if (myfile) 
    {
     // myfile.print(elapsedTime);
     DateTime now = rtc.now();
        myfile.print(now.year(), DEC);
        myfile.print('/');
        myfile.print(now.month(), DEC);
        myfile.print('/');
        myfile.print(now.day(), DEC);
        myfile.print(", ");
        myfile.print(now.hour(), DEC);
        myfile.print(':');
        myfile.print(now.minute(), DEC);
        myfile.print(':');
        myfile.print(now.second(), DEC);
      myfile.print(", ");
      myfile.print(T1,2);
      myfile.print(", ");
      myfile.print(T2,2);
      myfile.print(", ");
      myfile.print(T3);
      myfile.print(", ");
      myfile.print(T4);
      myfile.println(" ");
       myfile.print("\n");      
      myfile.close(); // close file      
      //Serial.println("Success"); //we print on the serial port, so we see that the writing to SD was OK. You can remove this
    } else {
      Serial.println("Could not open file (writing).");
    }  
  //}
  //else
  //{
    
  //}  
}

void loop()
{ 
  //ReadRTC();
  ReadSensors();
  PrintSerial();
  //PrintOLED();
  WriteSD();
  
  delay(30000);
}
