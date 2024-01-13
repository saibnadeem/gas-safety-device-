#include <SoftwareSerial.h>
#include <MQUnifiedsensor.h> // Specific library for MQ-135

// Pin connections (adjust based on your setup)
byte board = "UNO";  // Assuming Arduino Uno
const int mq135Pin = A0;
const int simRxPin = 0;
const int simTxPin = 1;
const int buzzerPin = 8;
const int ledPin = 9;

// Calibration values for MQ-135 (adjust based on your sensor)
float Ro = 10;  // Replace with your sensor's Ro value

// Threshold for gas detection (adjust as needed)
const int gasThreshold = 300;  // Example value

// Phone number for notifications
const char* phoneNumber = "123445";

MQUnifiedsensor mq135(String(board), mq135Pin, "MQ-135");
SoftwareSerial simModule(simRxPin, simTxPin);

void setup() {
  Serial.begin(9600);
  simModule.begin(9600);
  pinMode(buzzerPin, OUTPUT);
  pinMode(ledPin, OUTPUT);

  // Initialize MQ-135 sensor
  mq135.setRegressionMethod(1); // Set regression method (check library documentation)
  mq135.init();
  mq135.setRL(Ro); // Set Ro value

  // Initialize SIM module (replace with appropriate AT commands)
  simModule.println("AT");
  delay(1000);
  simModule.println("AT+CMGF=1");  // Set SMS mode to text
  delay(1000);
}

void loop() {
  mq135.update(); // Update sensor readings
  float ppm = mq135.readSensor("CO"); // Read CO concentration

  if (ppm > gasThreshold) {
    // Send SMS notification
    simModule.println("AT+CMGS=\"" + String(phoneNumber) + "\"");
    delay(1000);
    simModule.println("Gas leak detected! CO concentration: " + String(ppm) + " ppm");
    simModule.write(26);  // ASCII code for Ctrl+Z to end message

    // Optionally, make a call
    simModule.println("ATD" + String(phoneNumber) + ";");

    // Activate buzzer and LED
    digitalWrite(buzzerPin, HIGH);
    digitalWrite(ledPin, HIGH);
  } else {
    digitalWrite(buzzerPin, LOW);
    digitalWrite(ledPin, LOW);
  }

  delay(1000);  // Adjust delay as needed
}
