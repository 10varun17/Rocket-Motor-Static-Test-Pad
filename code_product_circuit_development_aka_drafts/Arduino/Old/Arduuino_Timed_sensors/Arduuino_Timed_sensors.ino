#include <OneWire.h>
#include <DallasTemperature.h>

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

float T1, T2; //Variables for the 4 thermometers

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
  
  //OLED part-----------------------------------------------------------------------------------
  //#if RST_PIN >= 0
  //display.begin(&Adafruit128x32, I2C_ADDRESS, RST_PIN);
  //#else // RST_PIN >= 0
  //display.begin(&Adafruit128x32, I2C_ADDRESS);
  //#endif // RST_PIN >= 0
  //Call oled.setI2cClock(frequency) to change from the default frequency.

  //display.setFont(System5x7);
  //display.set1X(); //set2x() is too large
  //display.clear();
  //--endofOLED----

  //SD card module-----------------------------------------------------------------------------------
  pinMode(CS, OUTPUT); // chip select pin is set as OUTPUT
  
  if (!SD.begin(CS)) 
  { // Initialize SD card
    Serial.println("No SD card found. Reset the device after inserting an SD card."); // if return value is false, something went wrong.
  }
    Serial.println("initialization done.");

  /* //We skip this part
  if (SD.exists("Temp.txt")) 
  { // if "Temp.txt" exists, fill will be deleted
    Serial.println("File exists.");
  if (SD.remove("Temp.txt") == true)
  {
      Serial.println("Successfully removed file.");
  }
  else 
  {
      Serial.println("Could not removed file.");
  }
  }
  */

  //Starting timer for the elapsed time
  startTime = millis();
  
}

void loop()
{ 
  //ReadSwitchState();
  ReadSensors();
  PrintSerial();
  //PrintOLED();
  WriteSD();
  
  delay(1000);
}

void ReadSensors()
{  
  elapsedTime = millis() - startTime;
  
  //Collect the values for each sensors    
  sensors.requestTemperatures(); //request the temperature

  //Filling up the variables
  T1 = sensors.getTempCByIndex(0);  
  T2 = sensors.getTempCByIndex(1);  
  //T3 = sensors.getTempCByIndex(2);  
  //T4 = sensors.getTempCByIndex(3);         
}



//void PrintOLED()
//{
  
  //128x32 OLED
  //1st line of the OLED
  //display.clear();  
  //display.setCursor(0, 0); //The cursor's unit is in pixels and not in blocks as in the case of the 16x2 LCD
  //display.print("1: ");
  //display.print(T1,1);
  //display.print("  2: ");
  //display.println(T2,1);

  //if(WritingEnabled == true)
  //{
    //display.setCursor(120, 0); //this display has to be positioned by pixel value and not block value!
    //display.print("W"); // W = writing is in progress
  //}
  //else
  //{
    //display.setCursor(120, 0); 
    //display.print("0"); // 0 = no writing at the moment    
  //}  

  //2nd line
  //display.setCursor(0, 1); 
  //display.print("3: ");
  //display.print(T3,1);
  //display.print("  4: ");
  //display.println(T4,1);

  //3rd line
  //display.setCursor(0, 2); 
  //display.print("t: ");
  //display.print(elapsedTime/1000);  
  //display.print(" s");
  
//}

void PrintSerial()
{
  Serial.print(elapsedTime); //time in ms
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
      myfile.print(elapsedTime);
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

//void ReadSwitchState()
//{
//if(digitalRead(FileOnOffButton) == HIGH) //if the button is high
//{
//  if(SwitchStatus == false) //if the previous status was false, we restart the timer to 0 by resetting the startTime.
//  {
//    SwitchStatus = true; //flip the state
//    startTime = millis(); //reset the timer to zero
//  }
//  WritingEnabled = true; //enable the writing
//  Serial.println("ON"); //message for checking the things out
//}
//else //in this case, the FileOnOffButton is LOW, which means, we don't write the SD card
//{
//  SwitchStatus = false; //Switch status is off
//  WritingEnabled = false; //we do not write on the SD card
//  Serial.println("OFF"); //message on the serial, to see what is happening
//}  
//}
