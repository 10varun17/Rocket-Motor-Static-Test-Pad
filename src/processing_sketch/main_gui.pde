import processing.serial.*;
Serial myPort;


void setup(){
  size(450, 500);
  myPort = new Serial(this, "COM5", 9600); // Starts the serial communication
  myPort.bufferUntil('\n'); // Defines up to which character the data from the serial port will be read. The character '\n' or 'New Line'
}


void draw(){
  background(237, 240, 241);
  fill(20, 160, 100); // Green Color
  stroke(33);
  strokeWeight(1);
  rect(100, 60, 250, 60, 10); // "START IGNITION" Button
  fill(255,0,0);
  rect(65, 140, 320, 60, 10); // "STOP DATA ACQUISITION" Button
  fill(0, 0, 255);
  rect(100, 220, 250, 60, 10); // "STOP IGNITION" Button
  fill(0,255,0);
  rect(50, 350, 150, 50, 10); // "Turn ON" Button
  fill(255,0,0);
  rect(250, 350, 150, 50, 10); // "Turn OFF" Button
  fill(255);
  
  textSize(28);
  text("START IGNITION",130, 100);
  text("STOP DATA ACQUISITION", 85, 180);
  text("STOP IGNITION",130, 260);
  text("Turn ON",73, 383);
  text("Turn OFF", 269, 383);
  textSize(24);
  fill(33);
  textSize(30);
  textSize(16);
  text("ROCKET MOTOR STATIC TEST BLUETOOTH CONTROL", 50, 40);
  text("FIRE EXTINGUISHER CONTROL BUTTONS", 90, 320);

  
  // If the button "START IGNITION" is pressed
  if(mousePressed && mouseX>100 && mouseX<350 && mouseY>60 && mouseY<120){
    myPort.write('1'); // Sends the character '1' to arduino, which then stops data acquisition.
    stroke(255,45,0);
    strokeWeight(2);
    noFill();
     rect(100, 60, 250, 60, 10);
  }
  // If the button "STOP DATA ACQUISITION" is pressed
  if(mousePressed && mouseX>65 && mouseX<385 && mouseY>140 && mouseY<200){
    myPort.write('0'); // Sends the character '0' to arduino, which then stops data acquisition.
    stroke(255,0,0);
    strokeWeight(2);
    noFill();
    rect(65, 140, 320, 60, 10);
  }
  // If the button "STOP IGNITION" is pressed
  if(mousePressed && mouseX>100 && mouseX<350 && mouseY>220 && mouseY<280){
    myPort.write('0'); // Sends the character 's' to arduino, which then stops data acquisition.
    stroke(255,0,0);
    strokeWeight(2);
    noFill();
    rect(100, 220, 250, 60, 10);
  }
   //If the button "Turn ON" is pressed 
    if(mousePressed && mouseX>50 && mouseX<200 && mouseY>350 && mouseY<400){
    myPort.write('o'); // Sends the character 'o' to arduino, which then turns on fire extinguisher.
    stroke(20,160,100);
    strokeWeight(2);
    noFill();
    rect(50, 350, 150, 50, 10);
  }
  // If the button "Turn OFF" is pressed
  if(mousePressed && mouseX>250 && mouseX<400 && mouseY>350 && mouseY<400){
    myPort.write('p'); // Sends the character 'p' to arduino, which then turns off fire extinguisher.
    stroke(255,0,0);
    strokeWeight(2);
    noFill();
    rect(250, 350, 150, 50, 10);
  }
}
