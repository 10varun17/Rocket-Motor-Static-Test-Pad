
/*  SD card Temperature  data logger
  This example shows how to log data from a DS18B20 temp sensor
  to an SD card using the SD library.
  Note : The max reliable run time is 49 days - due to limits on the millis() function used
  , if you want longer running time then change the code to use the delay() function instead
  The advantage of using millis is that the code is none blocking and you could do other tasks
  whilst waiting to perform readings.
 
  The circuit:   
   SD card attached to SPI bus as follows:
 ** MOSI - pin 11
 ** MISO - pin 12
 ** CLK - pin 13
 ** CS - pin 10 
 * 
 ** DB18B20
 * Data - pin 2, this must be pulled up to 5Vcc by 4K7 resistor
 
 By XTronical - see https://www.xtronical.com
 
  This example code is in the public domain. NO liability is accepted
  for its use.
 
*/
 
// Include the libraries we need
#include <OneWire.h>
#include <DallasTemperature.h>
#include <SPI.h>
#include <SD.h>
 
// Data wire is plugged into port 2 on the Arduino
#define ONE_WIRE_BUS 2
#define MINS_BETWEEN_READINGS 1
 
// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);
 
// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);
 
const int chipSelect = 10;  // for SD card
 
uint32_t LastTime=0;                // Last Time we took a reading (from ,millis())
String CoreFileName="TLog_";  // This id the core name of the file to write to, 
String FileName="";                 // The actual filename to write to based on the core name and a numeric addendum
 
void setup() {
  bool FileFound=true;        // See end of function
  uint16_t Index=0;
  
  // Open serial communications 
  // We use this just for error reporting if problems are occuring
  Serial.begin(9600);
 
  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    while (1);
  }
  sensors.begin();
    Serial.print("Locating devices...");
  Serial.print("Found ");
  deviceCount = sensors.getDeviceCount();
  Serial.print(deviceCount, DEC);
  Serial.println(" devices.");
  Serial.println("");
  // Work out the file name to use, we want to write to a different file each time
  // leaving the decision to erase files up to the operator, this way no data is lost
  // on accidental power cycles/reset etc.
  while(FileFound)
  {
    FileName=CoreFileName+String(Index)+".csv";         //.csv = Comma Seperated Values
    FileFound=SD.exists(FileName);
    Index++;
  }
  // If we get this far filename is a valid none existing file
    
  // locate devices on the bus

}
  TakeReading();  // Initial Reading
}
 
void loop() {
  if(millis()>LastTime+(MINS_BETWEEN_READINGS * 60000))       // This will only execute at the interval specified
    TakeReading();
   // You could do other stuff in the loop here if you wanted. Animate a display perhaps?
}
 
void TakeReading()
{
  String DataString = "";
  static uint16_t Time=0;
 
  sensors.requestTemperatures(); 
    for (int i = 0;  i < deviceCount;  i++)
  {
    Serial.print("Sensor ");
    Serial.print(i+1);
    Serial.print(" : ");
    tempC = sensors.getTempCByIndex(i);
    Serial.print(tempC);
    Serial.print((char)223);//shows degrees character
    Serial.print("C");
    
  }
  // Send the command to get temperatures
  // get temp of first sensor, you could expand this to show multiple
  DataString=String(Time);
  DataString+=",";
  DataString+=String(sensors.getTempCByIndex(0));
  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  File dataFile = SD.open(FileName, FILE_WRITE);
  // if the file is available, write to it:
  if (dataFile) {
    // If this is the first reading then put column headings in first
    if(Time==0)
      dataFile.println("Time (min),Temperature oC");  
    dataFile.println(DataString);
    dataFile.close();
  }
  else      // if the file isn't open, pop up an error:
  {
    Serial.print("error opening ");
    Serial.println(FileName);
  }
    
  LastTime=millis();    // update time of last reading 
  Time+=MINS_BETWEEN_READINGS;              // Next minute reading
}
