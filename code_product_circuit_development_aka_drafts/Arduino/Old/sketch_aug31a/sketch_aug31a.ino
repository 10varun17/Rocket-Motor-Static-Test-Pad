#include <LM35.h>
const int buzzer = 5;
float temp;
LM35 lm35(A0);
void setup(){
  pinMode(5,OUTPUT);
  Serial.begin(9600);
}
void loop(){
  lm35.getTemp(CELCIUS);
  temp = lm35.getTemp();
  Serial.println(temp);
  if (temp < 30){
    tone(buzzer,1000);
    delay(1000);
  }
  else {
    noTone(buzzer);
    delay(1000);
  }

}
