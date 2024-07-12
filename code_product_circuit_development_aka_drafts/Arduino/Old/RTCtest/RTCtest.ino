#include <OneWire.h>
#include <DallasTemperature.h>

#include "Wire.h"
#define DS3231_I2C_ADDRESS 0x68
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

float hour, minute, second, dayOfMonth, month, year, T1, T2; //Variables for the 4 thermometers

//const byte FileOnOffButton = 10; //toggle switch (ON-ON) for starting and stopping the logging 

//bool WritingEnabled = false; //we switch the status of this with the buttons
//bool SwitchStatus = false; //by default, this is false that means that the switch is OFF (no logging)

//unsigned long startTime;
//unsigned long elapsedTime;

//SSD1306AsciiAvrI2c display;

int CS = 4; //chip select pin for the MicroSD Card Adapter, This is the CS Pin
File myfile; // file object that is used to read and write data

// Convert decimal numbers to binary coded decimal
byte decToBcd(byte val)
{
  return ( (val / 10 * 16) + (val % 10) );
}

// Convert binary coded decimal to decimal numbers
byte bcdToDec(byte val)
{
  return ( (val / 16 * 10) + (val % 16) );
}

// Set the time
void setDS3231time(byte dayOfMonth, byte month, byte year,byte second, byte minute, byte hour)
{
  // sets time and date data to DS3231
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(0); // set next input to start at the seconds register
  Wire.write(decToBcd(second)); // set seconds
  Wire.write(decToBcd(minute)); // set minutes
  Wire.write(decToBcd(hour)); // set hours
  //Wire.write(decToBcd(dayOfWeek)); // set day of week (1=Sunday, 7=Saturday)
  Wire.write(decToBcd(dayOfMonth)); // set date (1 to 31)
  Wire.write(decToBcd(month)); // set month
  Wire.write(decToBcd(year)); // set year (0 to 99)
  Wire.endTransmission();
}

void readDS3231time(byte *dayOfMonth, byte *month, byte *year,byte *second, byte *minute, byte *hour)
{
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(0); // set DS3231 register pointer to 00h
  Wire.endTransmission();

  // request seven bytes of data from DS3231 starting from register 00h
  Wire.requestFrom(DS3231_I2C_ADDRESS, 7);
  *second = bcdToDec(Wire.read() & 0x7f);
  *minute = bcdToDec(Wire.read());
  *hour = bcdToDec(Wire.read() & 0x3f);
  //*dayOfWeek = bcdToDec(Wire.read());
  *dayOfMonth = bcdToDec(Wire.read());
  *month = bcdToDec(Wire.read());
  *year = bcdToDec(Wire.read());
}

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


  //SD card module-----------------------------------------------------------------------------------
  pinMode(CS, OUTPUT); // chip select pin is set as OUTPUT
  
  if (!SD.begin(CS)) 
  { // Initialize SD card
    Serial.println("No SD card found. Reset the device after inserting an SD card."); // if return value is false, something went wrong.
  }
    Serial.println("initialization done.");

  //Starting timer for the elapsed time
  //startTime = millis();
  Wire.begin();
  Serial.begin(9600);
  //setDS3231time(0,44,9,4,7,22);
}



void ReadSensors()
{  
//  elapsedTime = millis() - startTime;
  
  //Collect the values for each sensors    
  sensors.requestTemperatures(); //request the temperature

  //Filling up the variables
  T1 = sensors.getTempCByIndex(0);  
  T2 = sensors.getTempCByIndex(1);  
  //T3 = sensors.getTempCByIndex(2);  
  //T4 = sensors.getTempCByIndex(3);         
}




void PrintSerial()
{
  byte dayOfMonth, month, year, second, minute, hour;
  
  // retrieve data from DS3231
  readDS3231time(&dayOfMonth, &month,
                 &year,&second, &minute, &hour);
                 
  // send it to the serial monitor
  Serial.print(hour, DEC);
  // convert the byte variable to a decimal number when displayed
  Serial.print(":");
  if (minute < 10)
  {
    Serial.print("0");
  }
  
  Serial.print(minute, DEC);
  Serial.print(":");
  if (second < 10)
  {
    Serial.print("0");
  }
  
  Serial.print(second, DEC);
  Serial.print(" ");
  Serial.print(dayOfMonth, DEC);
  Serial.print("/");
  Serial.print(month, DEC);
  Serial.print("/");
  Serial.print(year, DEC);
  //Serial.print(elapsedTime); //time in ms
  Serial.print(" ");  //space
  Serial.print(T1,2); //temperature, 2 digits (i.e. 28.12)
  Serial.print(" ");  
  Serial.print(T2,2);
  //Serial.print(" ");  
  //Serial.print(T3,2);
  //Serial.print(" ");  
  //Serial.println(T4,2);
}

void WriteSD()
{
 // if(WritingEnabled == true)
  //{
    myfile = SD.open("Temp.txt", FILE_WRITE); // open "Temp.txt" to write data; make sure that you want to write in the same file that you created in the setup()
    myfile.seek(EOF); //This goes to the End Of (the) File and _continues_ writing from there
    if (myfile) 
    {
          myfile.print(hour, DEC);
  // convert the byte variable to a decimal number when displayed
  myfile.print(":");
  if (minute < 10)
  {
    myfile.print("0");
  }
  
  myfile.print(minute, DEC);
  myfile.print(":");
  if (second < 10)
  {
    myfile.print("0");
  }
  
  myfile.print(second, DEC);
  myfile.print(", ");
  myfile.print(dayOfMonth, DEC);
  myfile.print("/");
  myfile.print(month, DEC);
  myfile.print("/");
  myfile.print(year, DEC);
      //myfile.print(elapsedTime);
      myfile.print(", ");
      myfile.print(T1,2);
      myfile.print(", ");
      myfile.print(T2,2);
      //myfile.print(",");
      myfile.print("\n");
      //file.print(T3);
      //file.print(" ");
      //file.print(T4);
      //file.println(" ");      
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
  //ReadSwitchState();
  ReadSensors();
  PrintSerial();
  //PrintOLED();
  WriteSD();
  //displayTime();
  delay(1000);
}
