#include <OneWire.h>
#include <DallasTemperature.h>
#include <SPI.h>
#include <SD.h>

File myFile; 
const int oneWirePin = 2;

OneWire oneWireBus(oneWirePin);
DallasTemperature sensor(&oneWireBus);
 
void setup() {
  Serial.begin(9600);
  sensor.begin(); 
  discoverOneWireDevices();

  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.print("Initializing SD card...");

  if (!SD.begin(53)) {
    Serial.println("initialization failed!");
    while (1);
  }
  Serial.println("initialization done.");
}
 
void discoverOneWireDevices(void) {
  byte i;
  byte present = 0;
  byte data[12];
  byte addr[8];

  myFile = SD.open("test.txt", FILE_WRITE);
  Serial.println("Looking for 1-Wire devices");
  myFile.println("Looking for 1-Wire devices");
  while(oneWireBus.search(addr)) {
    Serial.println("1-Wire device found in this direcion");
    myFile.println("1-Wire device found in this direcion");
    
    for( i = 0; i < 8; i++) {
      Serial.print("0x");
      if (addr[i] < 16) {
        Serial.print('0');
      }
      Serial.print(addr[i], HEX);
      if (i < 7) {
        Serial.print(", ");
      }
    }
    if ( OneWire::crc8( addr, 7) != addr[7]) {
        Serial.println("Device error.");
        myFile.println("Device error.");
        return;
    }
  }
  Serial.println("Search completed.");
  myFile.println("Search completed.");
  oneWireBus.reset_search();
  myFile.close();
  return;
}
 
void loop() {
  myFile = SD.open("test.txt", FILE_WRITE);
  Serial.println("Reading temperatures: ");
  myFile.println("Reading temperatures: ");
  sensor.requestTemperatures();

  Serial.print("Temperature in sensor: ");
  Serial.print(sensor.getTempCByIndex(0));
  Serial.println(" ºC");
  myFile.print("Temperature in sensor: ");
  myFile.print(sensor.getTempCByIndex(0));
  myFile.println(" ºC");

  myFile.close();
  delay(1000); 
}
