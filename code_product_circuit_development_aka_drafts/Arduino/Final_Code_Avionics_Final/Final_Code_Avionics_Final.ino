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
#define ignition 6
#define fire 9

float temp_c, temp;
char Incoming_value;
char stopsignal;
int sw = 7;
int i = 10;
float mass;
float thrust;
float calibration_factor = -134715;
unsigned long startTime;
unsigned long elapsedTime;
boolean val = false;

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
     delay(1000);
     Serial.println("Please read control options before starting ignition.");

     //relay
     pinMode(yellow, OUTPUT);
     digitalWrite(yellow, HIGH);
     pinMode(ignition, OUTPUT);
     pinMode(fire, OUTPUT);
     digitalWrite(fire, LOW);
     Serial.println("Click 'START IGNITION' or type '1' on the serial monitor to start ignition.");
     delay(1000);
     Serial.println(" ");
     Serial.println("Click 'STOP DATA ACQUISITION' or type '0' on the serial monitor to stop data acquisition.");
     Serial.println(" ");
     delay(1000);
     Serial.println("o = Turn on fire extinguisher");
     Serial.println("p = Turn off fire extinguisher");
     delay(1000);
     Serial.println("Controlled Fire Extinguisher Enabled");
     Serial.println(" ");
     Serial.println("Now start testing rocket motor,");
     Serial.println(" ");

     //thermocouple
     pinMode(R_led,OUTPUT); 
     pinMode(G_led,OUTPUT); 
     pinMode(buzzer,OUTPUT);  
}

void loop()
{
  if (Serial.available()){
    Incoming_value = Serial.read();
    if(Incoming_value == 'o' || Incoming_value == 'O' ){
      digitalWrite(fire, HIGH);
    }
    else if(Incoming_value == 'p' || Incoming_value == 'P'){
      digitalWrite(fire, LOW);
    }  
    else if (digitalRead(yellow) == HIGH){
      if (Incoming_value == '1'){
        Serial.println("                  COUNTDOWN STARTED ");
        Serial.println(" ");
   
        for (int i=10; i>=1; i--){
          if (Serial.available()){
             Incoming_value = Serial.read();
          }
          if (Incoming_value != 's' && Incoming_value != 'S') {
            if(Incoming_value == 'o' || Incoming_value == 'O'){ digitalWrite(fire, HIGH); }
            if(Incoming_value == 'p' || Incoming_value == 'P'){digitalWrite(fire, LOW);}
            digitalWrite(yellow, !digitalRead(yellow));
            Serial.print("                   ");
            Serial.print(i);
            Serial.println("                   ");
            delay(100);//countdown
         }
         if(Incoming_value == 's' || Incoming_value == 'S'){
           Serial.println("IGNITION HALTED: Type '1' or click 'START IGNITION' button for restarting countdown.");
           digitalWrite(yellow, HIGH);
         }
       }
       if(Incoming_value != 's' && Incoming_value != 'S'){
         Serial.println(" "); 
         Serial.print("              ");
         Serial.println("GET SET GO");
         Serial.println(" ");
         digitalWrite(yellow,LOW);
         digitalWrite(ignition,HIGH);
         Serial.println("Relay is set to HIGH for 2 seconds.");
         Serial.println("Rocket Motor Igniting");
         delay(200);                       //ignition duration
         digitalWrite(ignition,LOW);
         Serial.println("Relay is set to LOW");
         Serial.println(" ");
         Serial.println("ROCKET MOTOR IGNITED");
         Serial.println("DATA ACQUISITION STARTED");
         Serial.println(); 
         startTime = millis(); 
         val = true;
       }
     }
     else {val = false;}
    }
   }else if (digitalRead(yellow) == LOW && Incoming_value == '0'){
     val = false;
     Serial.println("DATA ACQUISITION AND TESTING STOPPED");
     while(1){
      if (Serial.available()){
        Incoming_value = Serial.read();
        if (Incoming_value == 'o' || Incoming_value == 'O'){
          digitalWrite(fire, HIGH);
        }
        if (Incoming_value == 'p' || Incoming_value == 'P'){
          digitalWrite(fire, LOW);
        }
      }
     }
    } 
    if (val == true){
      scale.set_scale(calibration_factor);
      mass = scale.get_units(5);
      thrust = mass*9.8;
      elapsedTime = millis() - startTime;
      Serial.print(elapsedTime);
      Serial.print(" | ");
      Serial.print("Thrust = ");
      Serial.print(thrust, 5);
      Serial.print(" N");
      Serial.print(" | ");
      Serial.print("Temperature = ");
     temp_c = Thermocouple_read();
     temp = temp_c-25;
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
       digitalWrite(G_led, LOW);    
       digitalWrite(R_led, HIGH);  
     }
     else{  
       digitalWrite(G_led, HIGH);   
       digitalWrite(R_led, LOW);  
       digitalWrite(buzzer, LOW);
      }
      if (digitalRead(sw)== LOW){
        scale.set_scale();
        scale.tare();
      }
      Data = SD.open("DataAcquisition.txt", FILE_WRITE);
      Data.seek(EOF); 
      if (Data)
      {
        Data.print(elapsedTime);
        Data.print(", ");
        Data.print(thrust, 2);
        Data.print(", ");
        Data.print(temp, 2);
        Data.print("\n");
        Data.close();
        Serial.println("Data Written Successfully");
      }else{
         Serial.println("Couldn't write data");
         Serial.println(" ");
      }
      delay(20);
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
return NAN; 
}
v_out >>= 3;
return v_out*0.2529;
}



 
