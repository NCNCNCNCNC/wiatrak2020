#include "DFRobot_LedDisplayModule.h"
#include <TweenDuino.h>
#include <Servo.h>

#define POT_PIN A0
#define BUTTON_PIN 2
#define VALUES_COUNT 12

int values [] = {99, 99, 983, 983, 2827, 2827, 1515, 1515, 195, 195, 51, 51};
int angles [] = {30, 30, 60, 60, 90, 90, 120, 120, 150, 150, 180, 180};
int currentIndex = 0;
float value = 20.0;
float ang = 0.0;
int state = HIGH;      // the current state of the output pin
int reading;           // the current reading from the input pin
int previous = LOW;    // the previous reading from the input pin
long time = 0;         // the last time the output pin was toggled
long debounce = 200;   // the debounce time, increase if the output flickers
TweenDuino::Tween *valueTween;
TweenDuino::Tween *angleTween;

Servo servo;
DFRobot_LedDisplayModule LED(Wire, 0xE0);

unsigned long transitionDuration = 8000;
unsigned long pouseDuration = 3000;

boolean sequenceStarted = false;

void setup() {
  Serial.begin(9600);
  //LED.begin8();
  
//  while (LED.begin8() != 0)
//  {
//    Serial.println("Initialization of the chip failed, please confirm that the chip connection is correct!");
//    delay(1000);
//  }

  Serial.println("led ready");
  //delay(1000);
  //LED.setDisplayArea8(1,2,3,4,5,6,7,8);
  //LED.print8("H","A","L","O.","L","E","D","6");
  
  servo.attach(6);
  pinMode(POT_PIN, INPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  servo.write(value);
}


void checkButton() {
 
  reading = digitalRead(BUTTON_PIN);
  if (reading == HIGH && previous == LOW && millis() - time > debounce) {
    if (state == HIGH)
      state = LOW;
    else
      state = HIGH;
    time = millis();
  }
  previous = reading;
  
}

void startSequence() {
  valueTween = TweenDuino::Tween::to(value, transitionDuration, values[0]);
  angleTween = TweenDuino::Tween::to(ang, transitionDuration, angles[0]);
}

void loop() {

  checkButton();

  Serial.println(state);

  if ( state == LOW ) {

    if ( sequenceStarted == false ) { // !sequenceStarted
      startSequence();
      sequenceStarted = true;
    }

    valueTween->update(millis());
    angleTween->update(millis());

    if (valueTween->isComplete()) {

      Serial.println("DONE!");
      Serial.println(value);
      if (currentIndex < VALUES_COUNT - 1) {
        currentIndex ++;
      } else {
        currentIndex = 0;
      }

      unsigned long duration = 500UL;
      if (value == values[currentIndex]) {
        duration = pouseDuration;
      }
      else {
        duration = transitionDuration;
      }

      valueTween = TweenDuino::Tween::to(value,   duration, values[currentIndex]);
      angleTween = TweenDuino::Tween::to(ang, duration, angles[currentIndex]);
    }


    if (valueTween->isActive()) {
      LED.print8(value);
      servo.write(map(value, 50, 2827, 120, 250));
      Serial.println(value);
    }

    if (angleTween->isActive()) {
      servo.write(ang);
      //    servo.write( currentIndex * 30 );
      //Serial.println(ang);
    }

  }





  //  int val = analogRead(POT_PIN);
  //  int motorSpeed = map(val, 0, 1024, 0, 180);
  //  //  Serial.println(motorSpeed);
  //  servo.write(motorSpeed);

}
