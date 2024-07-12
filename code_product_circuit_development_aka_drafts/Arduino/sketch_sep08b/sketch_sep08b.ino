void setup(){
  pinMode(12, OUTPUT);
  Serial.begin(9600);
}

void loop(){
  if(Serial.read()== '1'){
    digitalWrite(12, HIGH);
    Serial.println("LED is ON");
  } else if (Serial.read()== '0'){
    digitalWrite(12, LOW);
    Serial.println("LED is OFF");
  }
}
