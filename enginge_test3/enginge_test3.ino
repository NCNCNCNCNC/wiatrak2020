#include <TweenDuino.h>
#include <Servo.h>
//#include "DFRobot_LedDisplayModule.h"
//DFRobot_LedDisplayModule LED(Wire, 0xE0);

#define MOTOR_PIN 6
//#define SERVO_PIN 3
#define MIN_MOTOR_SPEED 35
#define MAX_MOTOR_SPEED 150
//Servo servo;
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
//TweenDuino::Tween *angleTween;
boolean sequenceStarted = false;

int i = 0;

void setup() {

  //pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(MOTOR_PIN, OUTPUT);
  //pinMode(SERVO_PIN, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  digitalWrite(MOTOR_PIN, LOW);
  delay(10);

  motor.attach( MOTOR_PIN );
  motor.write(MIN_MOTOR_SPEED);
  //servo.attach( SERVO_PIN );
  //servo.write(0);

  Serial.begin(9600);

  //  /*Wait for the chip to be initialized completely, and then exit*/
  //  while (LED.begin8() != 0) {
  //    Serial.println("Initialization of the chip failed, please confirm that the chip connection is correct!");
  //    delay(1000);
  //  }
  //  /* Set the display area*/
  //  LED.setDisplayArea8(1, 2, 3, 4, 5, 6, 7, 8);
  //  LED.print8("H", "E", "L", "L", "0", "", "", "");


  digitalWrite(LED_BUILTIN, LOW);
  delay(8000);
  digitalWrite( LED_BUILTIN, HIGH );

}

void loop() {
  // put your main code here, to run repeatedly:
  //
  //int val = analogRead(A0);

  i++;
  if ( i >= 1023 ) {
    i = 0;
  }

  int motorSpeed = map(i, 0, 1024, MIN_MOTOR_SPEED, MAX_MOTOR_SPEED);
  Serial.println(motorSpeed);
  motor.write(motorSpeed);

  delay(50);

  // for (int i=0; i<180; i++){
  //
  //  servo.write(i);
  //  delay(100);
  // }


}
