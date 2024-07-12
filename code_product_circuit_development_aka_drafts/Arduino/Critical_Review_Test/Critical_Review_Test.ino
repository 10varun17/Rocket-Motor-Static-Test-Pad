#include "HX711.h"
#include <Wire.h>
#include <SD.h>

#define DOUT  A0
#define CLK  A1
#define SO  A4
#define CS  A2
#define SCK A3
#define G_led 2 
#define R_led 8
#define buzzer 3
#define yellow 5
#define relay 6

float temp_c, temp;
char Incoming_value = 0;
char stopsignal;
int sw = 7;
float mass;
float thrust;
float calibration_factor = -134715;
unsigned long startTime;
unsigned long elapsedTime;
boolean value = false;

HX711 scale(DOUT, CLK);

int CSK = 4;
File Data; //file object that is used to read and write data
 

void setup()
{
//load cell
Serial.begin(9600);
Serial.println("======= ROCKET MOTOR STATIC TEST =======");
Serial.println(" ");
pinMode(CLK, OUTPUT);
pinMode(sw, INPUT_PULLUP);
scale.set_scale();
scale.tare();
long zero_factor = scale.read_average();

//SD Card
pinMode(CSK, OUTPUT);
Serial.println("Initializing SD Card");
if (!SD.begin(CSK))
{
  Serial.println("NO SD CARD DETECTED. Please insert SD Card and reset the device");
  Serial.println(" ");
}else{
 Serial.println("Initialization Done"); 
 Serial.println(" ");
}

//relay
pinMode(yellow, OUTPUT);
pinMode(relay, OUTPUT);
Serial.println("Click `START IGNITION' button in mobile application or type `1' on the serial monitor to start ignition.");
delay(500);
Serial.println(" ");
Serial.println("Click `STOP DATA ACQUISITION' button in mobile application or type `0' on the serial monitor to stop data acquisition.");
Serial.println(" ");

//thermocouple
pinMode(R_led,OUTPUT); // declare Red LED as output
pinMode(G_led,OUTPUT); // declare Green LED as output
pinMode(buzzer,OUTPUT); // declare Buzzer as output 


}





void loop()
{
  
if (Serial.available()){
Incoming_value = Serial.read();
Serial.print (Incoming_value);
Serial.println(" ");

if (Incoming_value == '1'){
Serial.println(" ");

for (int c=10; c>=1; c--){
  if (Serial.available()){
  stopsignal = Serial.read();
  if (stopsignal != 's'){
    for (int i=10; i>=1; i--){  
digitalWrite(yellow, HIGH);
Serial.print("                   ");
Serial.print(i);
Serial.println("                   ");
delay(1000);//countdown
}
Serial.println(" ");
Serial.println("GET SET GO");
Serial.println(" ");
digitalWrite(yellow,LOW);
digitalWrite(relay,HIGH);
Serial.println("Relay is set to HIGH for 2 seconds.");
Serial.println(" ");
delay(2000);                       //ignition duration
digitalWrite(relay,LOW);
Serial.println("Relay is set to LOW");
Serial.println(" ");
Serial.println("ROCKET MOTOR IGNITED");
Serial.println("DATA ACQUISITION STARTED");
Serial.println(); 
startTime = millis(); //starting time for the elapsed timev
value = true;
} else {
  Serial.println("IGNITION HALTED");
  }

}
}
}

else {value = false;}
}

delay(100);
if (value == true && Incoming_value != '0'){
scale.set_scale(calibration_factor);
mass = scale.get_units(5);
thrust = mass*9.8;
elapsedTime = millis() - startTime;
Serial.print(elapsedTime); //time in ms
Serial.print(" | ");
Serial.print("Thrust = ");
Serial.print(thrust, 5);
Serial.print(" N");
Serial.print(" | ");
Serial.print("Temperature = ");
temp_c = Thermocouple_read();
temp = temp_c - 25;
if(isnan(temp_c)){
Serial.print("Connect");
Serial.println("Thermocouple");
loop();
}
Serial.print(temp,2);
Serial.print("C");
Serial.println();
    
if(temp>150){ 
digitalWrite(buzzer, HIGH);
digitalWrite(G_led, LOW); // Turn LED off.   
digitalWrite(R_led, HIGH);  // Turn LED on.
//delay(300);
}
else{  
digitalWrite(G_led, HIGH); // Turn LED on.  
digitalWrite(R_led, LOW);  // Turn LED off.
digitalWrite(buzzer, LOW);
}


if (digitalRead(sw)== LOW){
  scale.set_scale();
  scale.tare();
}
Data = SD.open("DataAcquisition.txt", FILE_WRITE);
Data.seek(EOF); //Goes to the end of the file and continues writing from there
if (Data)
{
  Data.print(elapsedTime);
  Data.print(", ");// comma is used to separate data so that the data can be exported to excel later
  Data.print(thrust, 2);
  Data.print(", ");
  Data.print(temp, 2);
  Data.print("\n");
  Data.close();
  Serial.println("Data Written Successfully");
} else{
  Serial.println("Couldn't write data");
  Serial.println(" ");
}
 delay(500);
}
else if (Incoming_value == '0'){
  Serial.println("DATA ACQUISITION STOPPED");
  while(1){}
}
}

double Thermocouple_read() {
uint16_t v_out;

pinMode(CS, OUTPUT);
pinMode(SO, INPUT);
pinMode(SCK, OUTPUT);
 
digitalWrite(CS, LOW);
delay(1); 
v_out = shiftIn(SO, SCK, MSBFIRST);
v_out <<= 8;
v_out |= shiftIn(SO, SCK, MSBFIRST);
 
digitalWrite(CS, HIGH);
if (v_out & 0x4) { 
//Thermocouple is disconnected
return NAN; 
}
 
// The lower three bits (0,1,2) are discarded status bits
v_out >>= 3;
// The remaining bits are the number of 0.25 degree (C) counts
return v_out*0.2529;
}

 
