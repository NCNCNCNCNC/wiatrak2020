#include <TweenDuino.h>
#include <Servo.h>
#include "DFRobot_LedDisplayModule.h"
DFRobot_LedDisplayModule LED(Wire, 0xE0);

#define MOTOR_PIN 6
#define SERVO_PIN 3
Servo servo;
Servo motor;

#define BUTTON_PIN 12
int btnState = HIGH;   // the current state of the output pin
int reading;           // the current reading from the input pin
int previous = LOW;    // the previous reading from the input pin
long time = 0;         // the last time the output pin was toggled
long debounce = 200;   // the debounce time, increase if the output flickers

#define VALUES_COUNT 12
float values [] = {99, 99, 983, 983, 2827, 2827, 1515, 1515, 195, 195, 51, 51};
float angles [] = {30, 30, 60, 60, 90, 90, 120, 120, 150, 150, 180, 180};
int currentIndex = 0;
float value = 0.0;
float ang = 0.0;

unsigned long transitionDuration = 5000;
unsigned long pauseDuration = 2000;

TweenDuino::Tween *valueTween;
TweenDuino::Tween *angleTween;
boolean sequenceStarted = false;

void setup() {

  pinMode(BUTTON_PIN, INPUT_PULLUP);
  
  servo.attach( SERVO_PIN );
  motor.attach( MOTOT_PIN );

  servo.write(0);
  motor.write(0);

  Serial.begin(115200);
  /*Wait for the chip to be initialized completely, and then exit*/
  while (LED.begin8() != 0) {
    Serial.println("Initialization of the chip failed, please confirm that the chip connection is correct!");
    delay(1000);
  }
  /* Set the display area*/
  LED.setDisplayArea8(1, 2, 3, 4, 5, 6, 7, 8);
  LED.print8("H", "E", "L", "L", "0", "", "", "");
  Serial.println( "hello" );
  delay(1000);

}

void startSequence() {
  Serial.println( "sequence started" );
  valueTween = TweenDuino::Tween::to(value, transitionDuration, values[0]);
  angleTween = TweenDuino::Tween::to(ang, transitionDuration, angles[0]);
  sequenceStarted = true;
}

void checkButton() {

  reading = digitalRead(BUTTON_PIN);
  if (reading == HIGH && previous == LOW && millis() - time > debounce) {
    if (btnState == HIGH)
      btnState = LOW;
    else
      btnState = HIGH;
    time = millis();
  }
  previous = reading;

}

void updateMotion() {

  unsigned long ms = millis();
  valueTween->update(ms);
  angleTween->update(ms);

  if (valueTween->isComplete()) {

    Serial.println("DONE!");
    Serial.println(value);
    if (currentIndex < VALUES_COUNT - 1) {
      currentIndex ++;
    } else {
      currentIndex = 0;
    }
    Serial.println(currentIndex);

    unsigned long duration = 500UL;
    if (value == values[currentIndex]) {
      duration = pauseDuration;
    }
    else {
      duration = transitionDuration;
    }

    //valueTween->restartFrom(ms);

    delete valueTween;
    delete angleTween;
    
    valueTween = TweenDuino::Tween::to(value, duration, values[currentIndex]);
    angleTween = TweenDuino::Tween::to(ang, duration, angles[currentIndex]);

    Serial.println( "ping" );
    
  }

  if (valueTween->isActive()) {
    LED.print8(value);
    motor.write(map(value, 51, 2827, 0, 180));
    //Serial.println(value);
  }

  if (angleTween->isActive()) {
    servo.write(ang);
  }


}

void loop() {

  checkButton();

  if ( btnState == HIGH && !sequenceStarted) {
    startSequence();
  }

  if ( sequenceStarted ) {
    updateMotion();
  } else {
    //LED.print8("-", "-", "-", "-", "-", "-", "-", "-");
  }

}
