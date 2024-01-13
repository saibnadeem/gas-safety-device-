#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>
#include <Servo.h>  // Include Servo library for motor control
Servo motor;  // Create servo object for motor control
SoftwareSerial sim800l(2, 3); // RX, TX for Sim800l
LiquidCrystal_I2C lcd(0x3F, 16, 2); // I2C address 0x27, 16 columns, 2 rows
const int motorPin = 6;  // Pin for motor control (adjust as needed)
const int mq135Pin = A0;
const int ledPin = 13;
const int buzzerPin = 9;

int airQualityThreshold = 100;

void setup() {
  lcd.begin(16, 2);
  pinMode(mq135Pin, INPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  motor.attach(motorPin);  // Attach motor to pin
  lcd.init();
lcd.backlight();
  lcd.print("Air Quality:");
  sim800l.begin(9600);
  
  // You may need to initialize your SIM module here.
}

void loop() {
  int airQuality = analogRead(mq135Pin);

  lcd.setCursor(0, 1);
  lcd.print("     "); // Clear previous reading
  lcd.setCursor(0, 1);
  lcd.print(airQuality);

  if (airQuality > airQualityThreshold) {
    sendSMS("+91123161544", "High air quality alert!");
    makeCall();
    activateAlarm();
    rotateMotor();  
  } else {
    deactivateAlarm();
  }

  delay(1000); // Adjust delay according to your needs
}

void sendSMS(String phoneNumber, String message) {
  sim800l.print("AT+CMGF=1\r");  // Set SMS mode to text
  delay(5000);
  sim800l.print("AT+CMGS=\"");
  sim800l.print(phoneNumber);
  sim800l.println("\"");
  delay(5000);
  sim800l.println(message);
  delay(5000);
  sim800l.println((char)26);  // Send SMS command
  delay(5000);
}

void makeCall() {
  sim800l.println("ATD+911231456494890;"); // Replace with your number
  delay(100); // Adjust the delay based on your needs
}

void activateAlarm() {
  digitalWrite(ledPin, HIGH);
  digitalWrite(buzzerPin, HIGH);
}
 void rotateMotor() {
  motor.write(180);  // Rotate motor 180 degrees
  delay(500);  // Adjust delay based on motor speed
  motor.write(0);  // Return motor to initial position (optional)
}
void deactivateAlarm() {
  digitalWrite(ledPin, LOW);
  digitalWrite(buzzerPin, LOW);
}
