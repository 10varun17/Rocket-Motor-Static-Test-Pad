#include "HX711.h"
#include <Wire.h>
#include <SD.h>

#define DOUT  A0
#define CLK   A1
#define SO    A4
#define CS    A2
#define SCK   A3
#define G_led 2 
#define R_led 8
#define buzzer 3
#define yellow 5
#define ignition 6
#define fire 9

float temp_c, temp; // Variables for temperature readings
char Incoming_value; // Variable to store incoming serial commands
char stopsignal; // Unused variable
int sw = 7; // Switch pin
int i = 10; // Countdown variable
float mass; // Mass measurement variable
float thrust; // Thrust calculation variable
float calibration_factor = -134715; // Calibration factor for load cell
unsigned long startTime; // Start time for data acquisition
unsigned long elapsedTime; // Elapsed time during data acquisition
boolean val = false; // Flag to control data acquisition loop

HX711 scale(DOUT, CLK); // HX711 load cell instance

int CSK = 4; // Chip select pin for SD card
File Data; // File object for SD card operations

void setup()
{
    // Initialize serial communication
    Serial.begin(9600);
    Serial.println("======= ROCKET MOTOR STATIC TEST =======");
    Serial.println(" ");
    
    // Initialize pins for load cell
    pinMode(CLK, OUTPUT);
    pinMode(sw, INPUT_PULLUP);
    scale.set_scale();
    scale.tare();
    long zero_factor = scale.read_average();
    
    // Initialize SD card
    pinMode(CSK, OUTPUT);
    Serial.println("Initializing SD Card");
    if (!SD.begin(CSK))
    {
        Serial.println("NO SD CARD DETECTED. Please insert SD Card and reset the device");
        Serial.println(" ");
    }
    else
    {
        Serial.println("Initialization Done"); 
        Serial.println(" ");
    }
    delay(1000);
    Serial.println("Please read control options before starting ignition.");
    
    // Initialize pins for relay and other components
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
    delay(2000);
    Serial.println("Controlled Fire Extinguisher Enabled");
    Serial.println(" ");
    Serial.println("Now start testing rocket motor,");
    Serial.println(" ");
    
    // Initialize pins for thermocouple and LED indicators
    pinMode(R_led, OUTPUT); 
    pinMode(G_led, OUTPUT); 
    pinMode(buzzer, OUTPUT);  
}

void loop()
{
    // Check for incoming commands from serial monitor
    if (Serial.available())
    {
        Incoming_value = Serial.read();
        // Command to turn on fire extinguisher
        if (Incoming_value == 'o' || Incoming_value == 'O')
        {
            digitalWrite(fire, HIGH);
        }
        // Command to turn off fire extinguisher
        else if (Incoming_value == 'p' || Incoming_value == 'P')
        {
            digitalWrite(fire, LOW);
        }  
        // Start countdown if '1' is received and yellow LED is high
        else if (digitalRead(yellow) == HIGH)
        {
            if (Incoming_value == '1')
            {
                Serial.println("                  COUNTDOWN STARTED ");
                Serial.println(" ");
                // Countdown loop
                for (int i = 10; i >= 1; i--)
                {
                    if (Serial.available())
                    {
                        Incoming_value = Serial.read();
                    }
                    // Check if 's' is received to stop ignition
                    if (Incoming_value != 's' && Incoming_value != 'S')
                    {
                        // Control fire based on commands
                        if (Incoming_value == 'o' || Incoming_value == 'O')
                        {
                            digitalWrite(fire, HIGH);
                        }
                        if (Incoming_value == 'p' || Incoming_value == 'P')
                        {
                            digitalWrite(fire, LOW);
                        }
                        digitalWrite(yellow, !digitalRead(yellow));
                        Serial.print("                   ");
                        Serial.print(i);
                        Serial.println("                   ");
                        delay(1000); // Countdown delay
                    }
                    // Stop ignition if 's' is received
                    if (Incoming_value == 's' || Incoming_value == 'S')
                    {
                        Serial.println("IGNITION HALTED: Type '1' or click 'START IGNITION' button for restarting countdown.");
                        digitalWrite(yellow, HIGH);
                    }
                }
                // Proceed with ignition if not stopped
                if (Incoming_value != 's' && Incoming_value != 'S')
                {
                    Serial.println(" "); 
                    Serial.print("              ");
                    Serial.println("GET SET GO");
                    Serial.println(" ");
                    digitalWrite(yellow, LOW);
                    digitalWrite(ignition, HIGH);
                    Serial.println("Relay is set to HIGH for 2 seconds.");
                    Serial.println("Rocket Motor Igniting");
                    delay(2000); // Ignition duration
                    digitalWrite(ignition, LOW);
                    Serial.println("Relay is set to LOW");
                    Serial.println(" ");
                    Serial.println("ROCKET MOTOR IGNITED");
                    Serial.println("DATA ACQUISITION STARTED");
                    Serial.println(); 
                    startTime = millis(); // Record start time for data acquisition
                    val = true; // Enable data acquisition flag
                }
            }
            else
            {
                val = false; // If '1' is not received, disable data acquisition
            }
        }
    }
    // Stop data acquisition if '0' is received and yellow LED is low
    else if (digitalRead(yellow) == LOW && Incoming_value == '0')
    {
        val = false;
        Serial.println("DATA ACQUISITION AND TESTING STOPPED");
        // Stay in loop until reset if '0' is received
        while (1)
        {
            if (Serial.available())
            {
                Incoming_value = Serial.read();
                // Control fire based on commands
                if (Incoming_value == 'o' || Incoming_value == 'O')
                {
                    digitalWrite(fire, HIGH);
                }
                if (Incoming_value == 'p' || Incoming_value == 'P')
                {
                    digitalWrite(fire, LOW);
                }
            }
        }
    }
    
    // Perform data acquisition if val is true
    if (val == true)
    {
        scale.set_scale(calibration_factor); // Set scale with calibration factor
        mass = scale.get_units(5); // Get mass measurement
        thrust = mass * 9.8; // Calculate thrust
        elapsedTime = millis() - startTime; // Calculate elapsed time
        Serial.print(elapsedTime);
        Serial.print(" | ");
        Serial.print("Thrust = ");
        Serial.print(thrust, 5);
        Serial.print(" N");
        Serial.print(" | ");
        Serial.print("Temperature = ");
        temp_c = Thermocouple_read(); // Read temperature from thermocouple
        if (isnan(temp_c))
        {
            Serial.print("Connect");
            Serial.println("Thermocouple");
            loop(); // Restart loop if thermocouple reading is invalid
        }
        Serial.print(temp_c, 2);
        Serial.print("C");
        Serial.println();   
        
        // Control LED and buzzer based on temperature threshold
        if (temp_c > 150)
        { 
            digitalWrite(buzzer, HIGH);
            digitalWrite(G_led, LOW);    
            digitalWrite(R_led, HIGH);  
        }
        else
        {  
            digitalWrite(G_led, HIGH);   
            digitalWrite(R_led, LOW);  
            digitalWrite(buzzer, LOW);
        }
        
        // Reset scale if switch is pressed
        if (digitalRead(sw) == LOW)
        {
            scale.set_scale();
            scale.tare();
        }
        
        // Write data to SD card
        Data = SD.open("DataAcquisition.txt", FILE_WRITE);
        Data.seek(EOF); // Move cursor to end of file
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
        }
        else
        {
            Serial.println("Couldn't write data");
            Serial.println(" ");
        }
        
        delay(100);
    }
}

// Function to read temperature from thermocouple
double Thermocouple_read()
{
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
        return NAN; // Return NaN if thermocouple connection issue
    }
    v_out >>= 3;
    return v_out * 0.2529; // Convert thermocouple reading to Celsius
}