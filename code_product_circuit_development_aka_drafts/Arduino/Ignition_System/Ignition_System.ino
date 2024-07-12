#define led 8
#define relay 6



void setup() {
  // put your setup code here, to run once:
pinMode(8, OUTPUT);
pinMode(6, OUTPUT);
digitalWrite(8, HIGH);
delay(2000);
digitalWrite(8,LOW);
digitalWrite(6,HIGH);
delay(1000);
digitalWrite(6,LOW);

}

void loop() {
  // put your main code here, to run repeatedly:

}
