#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 7, 6, 5, 4);

// Pin configuration
const int buttonPin = 2; 
const int irSensorPin = 3; 
const int ledPin = 8;
const int echoPin = 9;    
const int triggerPin = 10;


volatile bool isLedOn = false; 
volatile unsigned long lastDetectionTime = 0; 
const unsigned long debounceDelay = 200; // 200 ms ... The natural human cannot press the button more than once in 200ms 
										 // so we take only one pressed every 200ms
  
void setup() {
  //Pin modes
  pinMode(buttonPin, INPUT_PULLUP);
  digitalWrite(buttonPin, HIGH); 	
  pinMode(irSensorPin, INPUT);   
  pinMode(ledPin, OUTPUT);
  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INPUT);
  
  // Attach interrupt routines to button and IR sensor pins
  attachInterrupt(digitalPinToInterrupt(buttonPin), buttonInterrupt, FALLING); 
  attachInterrupt(digitalPinToInterrupt(irSensorPin), irSensorInterrupt, FALLING);
  
  // Initialize LCD 
  lcd.begin(16, 2); 
  Serial.begin(9600);
}

void loop() {
  if (millis() - lastDetectionTime > 3000) {
    lcd.setCursor(0, 1);
    lcd.print("                ");
  }

  // Trigger the ultrasonic pulse
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);

  // Measure the duration of the pulse using pulseIn function
  long duration = pulseIn(echoPin, HIGH); // Jubran Samuel Ramez Jihad

  // Calculate the distance in cm
  float distance_cm = duration * 0.0343 / 2; // 343 m is speed of sound
  											// devided by 2 because go forward and go back 

  // Convert the distance to a string and print it on Serial and LCD
  char distanceStr[10]; 
  dtostrf(distance_cm, 5, 2, distanceStr);
  Serial.println(distanceStr);
  Serial.println(distance_cm);

  // Display the distance on the LCD display
  lcd.setCursor(0, 0);
  lcd.print("                ");
  lcd.setCursor(0, 0);
  lcd.print("Distance: ");
  lcd.print(distanceStr);
  
  // Wait for 3 seconds before the next measurement
  delay(3000); 
}

// Interruption for button when pressed
void buttonInterrupt() {
  static unsigned long lastInterruptTime = 0;
  unsigned long interruptTime = millis();

  if (interruptTime - lastInterruptTime > debounceDelay) {
    if (isLedOn) {
      isLedOn = false;
      digitalWrite(ledPin, LOW);
    } else {
      isLedOn = true;
      digitalWrite(ledPin, HIGH);
    }
  }
  lastInterruptTime = interruptTime;
}

// Interruption for ir sensor 
void irSensorInterrupt() {
  lastDetectionTime = millis();
  lcd.setCursor(0, 1); 
  lcd.print("Object Detected"); 
}
