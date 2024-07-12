  int lm35_pin = A0;  /* LM35 O/P pin */
  int temp_adc_val;
  float temp_val;
  int buzzer = 5;

void setup() {  
  pinMode(lm35_pin,INPUT);
  pinMode(7, OUTPUT);
  pinMode(buzzer,OUTPUT);
  Serial.begin(9600); // initialize serial communication:
 
}

void loop() {
  // put your main code here, to run repeatedly:
  temp_adc_val = analogRead(lm35_pin);  /* Read Temperature temp_adc_val=analog to digital converted value  */
                  
  //Serial.print("ADC = ");
  //Serial.println(temp_adc_val);
  
  temp_val = (temp_adc_val * 4.88); // Convert adc value to equivalent voltage  0.00488 volts to 4.88 mV 

  temp_val = (temp_val/10); // LM35 gives output of 10mv/°C 
  //Serial.print("Temperature = ");
  //Serial.print(temp_val);
  //Serial.print(" Degree Celsius");
  //Serial.println(); 
  if(temp_val<30){
      Serial.print(temp_val);
  //Serial.print(" Degree Celsius");
  Serial.println(); 
    digitalWrite(7,HIGH);
   tone(buzzer,1000);
    //delay(1000);
  }
  else if(temp_val>55){
      Serial.print(temp_val);
  //Serial.print(" Degree Celsius");
  Serial.println(); 
      digitalWrite(7,HIGH);
      tone(buzzer,1000);
 
  }
    Serial.print(temp_val);
  //Serial.print(" Degree Celsius");
  Serial.println(); 
  digitalWrite(7,LOW);
  noTone(buzzer);
  }
  
