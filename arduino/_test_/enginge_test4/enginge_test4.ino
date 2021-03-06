#include <TweenDuino.h>
#include <Servo.h>
#include "DFRobot_LedDisplayModule.h"
DFRobot_LedDisplayModule LED(Wire, 0xE0);
Servo motor;
Servo servo;

#define MOTOR_PIN 6
#define SERVO_PIN 3
#define MIN_MOTOR_SPEED 45
#define MAX_MOTOR_SPEED 140
#define MOTOR_INIT_VAL 30

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

  //pinMode(A0, INPUT);
  pinMode(MOTOR_PIN, OUTPUT);
  pinMode(SERVO_PIN, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  //digitalWrite(MOTOR_PIN, LOW);
  //delay(10);
  
  motor.attach(MOTOR_PIN);
  motor.write(MOTOR_INIT_VAL); // nie moze byc zero
  servo.attach(SERVO_PIN);
  servo.write(0);

  Serial.begin(9600);

  /*Wait for the chip to be initialized completely, and then exit*/
  while (LED.begin8() != 0) {
    Serial.println("Initialization of the chip failed, please confirm that the chip connection is correct!");
    delay(1000);
  }
  /* Set the display area*/
  LED.setDisplayArea8(1, 2, 3, 4, 5, 6, 7, 8);
  LED.print8("H", "E", "L", "L", "0", "", "", "");
  
  digitalWrite( LED_BUILTIN, LOW );
  delay(8000);
  digitalWrite( LED_BUILTIN, HIGH );

  startSequence();
  
}

void startSequence() {
  Serial.println( "sequence started" );
  valueTween = TweenDuino::Tween::to(value, transitionDuration, values[0]);
  angleTween = TweenDuino::Tween::to(ang, transitionDuration, angles[0]);
  sequenceStarted = true;
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

    delete valueTween;
    delete angleTween;
    
    valueTween = TweenDuino::Tween::to(value, duration, values[currentIndex]);
    angleTween = TweenDuino::Tween::to(ang, duration, angles[currentIndex]);
    
  }

  if (valueTween->isActive()) {
    LED.print8(int(value));
    int motorSpeed = map(value, 51, 2827, MIN_MOTOR_SPEED, MAX_MOTOR_SPEED);
    motor.write(motorSpeed);
    Serial.println(motorSpeed);
  }

  if (angleTween->isActive()) {
    servo.write(ang);
  }


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

void loop() {
  // put your main code here, to run repeatedly:
  //
  checkButton();
  
  if ( sequenceStarted ) {
    updateMotion();
  } else {
    LED.print8("-", "-", "-", "-", "-", "-", "-", "-");
  }


}
