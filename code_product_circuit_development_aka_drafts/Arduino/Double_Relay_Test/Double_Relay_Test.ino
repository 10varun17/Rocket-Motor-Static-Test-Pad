#define relay1 13
#define relay2 12
char value = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
pinMode(relay1, OUTPUT);
pinMode(relay2, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available()){
    value = Serial.read();
    Serial.println(value);
    if (value == '1'){
      digitalWrite(relay1, HIGH);
    }
    else if (value == '0'){
      digitalWrite(relay2, HIGH);
    }
    else{
      digitalWrite(relay1, LOW);
      digitalWrite(relay2, LOW);
    }
  }

}
