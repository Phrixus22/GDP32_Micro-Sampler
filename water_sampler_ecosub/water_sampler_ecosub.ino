// Includes
#include <Wire.h>
#include <Servo.h>
#include <SoftwareSerial.h>

// Select communication protocol to use to control the sampler
// Choices: terminal, rs232, i2c, pwm
#define COMM_PROTO "rs232"

// Sampler Design
#define SAMPLES 6
#define ROTATION_DISTANCE 32.5
#define BAUD_RATE 9600

// Global constants
// Do not edit these
const String VERSION = "v2.1";

const int SERVO_PIN = 9;
const int COMM_PIN_1 = A4;
const int COMM_PIN_2 = A5;
const int I2C_ADDRESS = 0x09;

// Global variables
// Do not edit these
int collectedSamples = 0;

float servoPosition = 0;

Servo samplerServo;

void setup (){
  // Servo Setup
  samplerServo.attach(SERVO_PIN);
  samplerServo.write(servoPosition);

  if (COMM_PROTO == "rs232" || COMM_PROTO == "terminal"){
    // Open serial communications and wait for port to open:
    Serial.begin(BAUD_RATE);
    while (!Serial) {
      ; // wait for serial port to connect. Needed for Native USB only
    }
    if (COMM_PROTO == "terminal"){
      Serial.print("Water Sampler, Software Version ");
      Serial.println(VERSION);
      Serial.println("Options:");
      Serial.println("\t$sample - Takes Sample");
      Serial.println("\t$reset  - Reset Sampler");
      Serial.println(">>> ");
    }
  }

  if (COMM_PROTO == "i2c"){
    Wire.begin(I2C_ADDRESS);
    // i2c event handlers
    Wire.onRequest(collectSample);   // register an event handler for data requests
  }

  if (COMM_PROTO == "pwm"){
    // NEED TO IMPLEMENT SOFTWARE PWM
  }
}

void loop (){
  if (COMM_PROTO == "rs232" || COMM_PROTO == "terminal"){
    while (Serial.available() == 0) {}     //wait for data available
    String recievedCommand = Serial.readString();  //read until timeout
    recievedCommand.trim();                        // remove any \r \n whitespace at the end of the String
    if (recievedCommand == "$sample") {
      collectSample();
      Serial.println("$ok"); // Currently sends okay regardless of if sample is collected or not
      if (COMM_PROTO == "terminal"){
        Serial.print("Sample Collected: ");
        Serial.print(collectedSamples);
        Serial.print("/");
        Serial.println(SAMPLES);
      }
    } 
    if (recievedCommand == "$reset") {
      // Reset all values and put servo to zero position
      servoPosition = 0.0;
      samplerServo.write(servoPosition);
      collectedSamples = 0;
      Serial.println("$ok");
      if (COMM_PROTO == "terminal"){
        Serial.println("Sampler Reset");
      }
    }
  }

  if (COMM_PROTO == "pwm"){
    // NEED TO IMPLEMENT SOFTWARE PWM
  }
}

void collectSample(){
  if (collectedSamples < SAMPLES){
    servoPosition += ROTATION_DISTANCE; // Add rotation distance to ammount servo moved
    samplerServo.write(servoPosition); // Move Servo
    collectedSamples++; // Increase Count
  }
  if (COMM_PROTO == "i2c"){
    Wire.write("$ok");
  }
}
