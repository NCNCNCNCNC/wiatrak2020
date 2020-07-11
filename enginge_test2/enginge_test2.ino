#include <Servo.h>
#define POT_PIN A0
#define BUTTON_PIN 2
int state = HIGH;      // the current state of the output pin
int reading;           // the current reading from the input pin
int previous = LOW;    // the previous reading from the input pin
long time = 0;         // the last time the output pin was toggled
long debounce = 200;   // the debounce time, increase if the output flickers
Servo servo;

void setup() {
  // put your setup code here, to run once:
  servo.attach(6);
  pinMode(POT_PIN, INPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  Serial.begin(9600);
}

void loop() {
  reading = digitalRead(BUTTON_PIN);
  if (reading == HIGH && previous == LOW && millis() - time > debounce) {
    if (state == HIGH)
      state = LOW;
    else
      state = HIGH;

    time = millis();
  }
  
  previous = reading;
  Serial.println(state);
  
  int val = analogRead(POT_PIN);
  int motorSpeed = map(val, 0, 1024, 0, 180);
  //  Serial.println(motorSpeed);
  servo.write(motorSpeed);

}
