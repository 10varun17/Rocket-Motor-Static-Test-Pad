//  THE TEAM OF E INFOTAINMENT   
      //https://www.youtube.com/channel/UCr688fGR4aI_tWYD-YKYrzQ
      //https://www.youtube.com/channel/UCr688fGR4aI_tWYD-YKYrzQ
                 //please subscribe to my youtube channel
#include "HX711.h"
#include <Wire.h>
#include <LiquidCrystal.h>

LiquidCrystal lcd(7, 6, 5, 4, 3, 2);
 
#define DOUT  A0
#define CLK  A1
 
HX711 scale(DOUT, CLK);

int sw = 8;
float mass;
float thrust;
float calibration_factor = -134715;
 

void setup()
{
  Serial.begin(9600);
//pinMode(SCK, OUTPUT);
pinMode(sw, INPUT_PULLUP);
scale.set_scale();
scale.tare();
long zero_factor = scale.read_average();

lcd.begin(16, 2);
lcd.print(" Thrust ");
lcd.setCursor(0,1);
lcd.print(" Measurement ");
delay(100);
lcd.clear();
//calibrate();
}
 
void loop()
{
scale.set_scale(calibration_factor);
mass = scale.get_units(5);
thrust = mass*9.8;

lcd.setCursor(0,0);
lcd.print("Measured Thrust");
lcd.setCursor(0,1);
lcd.print(thrust);
lcd.print(" N");
delay(100);
//lcd.clear();

Serial.print("Measured Thrust: ");
Serial.print(thrust);
Serial.println(" N");
Serial.println();

if (digitalRead(sw)== LOW){
  scale.set_scale();
  scale.tare();
}
 

}
 
