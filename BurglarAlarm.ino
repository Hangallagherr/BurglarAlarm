// Our Internet of Things object, which is a Buglar Alarm which is detected by sound using the sound sensor. When sound is detected avove a certain threshold it triggers the buzzer (to ...
// make an alarm like sound) it aslo turns on the LED. When sound is detected it should aslo display "intruder" on the LCD and start flashing red. If no sound is detected the LCD will be blue
// and display "No intruder", the LED will not be turned on and the Buzzer will not be triggered.
#include <Wire.h>
#include "rgb_lcd.h"

rgb_lcd lcd;

const int soundSensorPin = A0; 
const int ledPin = 7; 
const int buzzerPin = 8; 

int soundThreshold = 400; // Adjust this accoridng to how easily we want the sound to be detected (might be easier to not lower it in class room becuse then it will be consistant)
bool intruderDetected = false;
unsigned long buzzerStartTime = 0;
const unsigned long buzzerInterval = 200; // if the sound that is triggering the sound sensor is consistant it the buzzer will sound alarm like

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  lcd.begin(16, 2);
  lcd.setRGB(0, 0, 255); // Set backlight color to blue
  lcd.setCursor(0, 0);
  lcd.print("No Intruder");
}

void loop() {
  int soundValue = analogRead(soundSensorPin);

  if (soundValue > soundThreshold) {
    // when sound detected, trigger buzzer, light LED, set LCD display
    if (!intruderDetected) {
      intruderDetected = true;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Intruder");
      lcd.setRGB(255, 0, 0); // Set backlight color to red (intruder is detected if red)
      digitalWrite(ledPin, HIGH);
      buzzerStartTime = millis();
    }
  } else {
    // If there is no sound, turn off LED, buzzer, and reset the LCD display
    if (intruderDetected) {
      intruderDetected = false;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("No Intruder");
      lcd.setRGB(0, 0, 255); // Set backlight color to blue (no sound=nointruder=blue)
      digitalWrite(ledPin, LOW);
      if (millis() - buzzerStartTime >= buzzerInterval) {
        noTone(buzzerPin); // Stop the buzzer after the interval
      }
    }
  }
  
  if (intruderDetected && millis() - buzzerStartTime >= buzzerInterval) {
    tone(buzzerPin, 1000); // alarm like sound
  }
  
  delay(100);
}
