#include <Wire.h>
#include <SD.h>
#include "RTClib.h"
RTC_DS1307 rtc;
#define PIN_SPI_CS 4
#define FILE_NAME "time.txt"
File myfile;


void setup () {
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
if (!SD.begin(PIN_SPI_CS)){
  Serial.println(F("SD CARD FAILED"));
  while(1);
}
}

void loop () {
  //open file for writing
  myfile = SD.open(FILE_NAME,FILE_WRITE);
  if (myfile){
    Serial.println(F("Writing log to SD CARD"));
    //write timestamp
DateTime now = rtc.now();
myfile.print(now.year(), DEC);
myfile.print('/');
myfile.print(now.month(), DEC);
myfile.print('/');
myfile.print(now.day(), DEC);
myfile.print(" ");
myfile.print(now.hour(), DEC);
myfile.print(':');
myfile.print(now.minute(), DEC);
myfile.print(':');
myfile.print(now.second(), DEC);
myfile.println();
  }
  else{
    Serial.print(F("SD CARD ERROR"));
    Serial.println(FILE_NAME);
}
delay(1000);
}
