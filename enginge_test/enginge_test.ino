#include <TweenDuino.h>
#include <Servo.h>
Servo motor;

#define MOTOR_PIN 6
#define MIN_SPEED 35
#define MAX_SPEED 150

int i = 0;

TweenDuino::Tween *valueTween;

void setup() {

  //pinMode(A0, INPUT);
  pinMode(MOTOR_PIN, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  digitalWrite(MOTOR_PIN, LOW);
  delay(10);
  
  motor.attach(MOTOR_PIN);
  motor.write(MIN_SPEED);

  Serial.begin(9600);
  
  digitalWrite( LED_BUILTIN, LOW );
  delay(8000);
  digitalWrite( LED_BUILTIN, HIGH );
}

void loop() {
  // put your main code here, to run repeatedly:
  //
  int val = analogRead(A0);

  i++;
  if( i >= 1023 ){
    i = 0;
  }
  
  int motorSpeed = map(i, 0, 1024, MIN_SPEED, MAX_SPEED);
  Serial.println(motorSpeed);
  motor.write(motorSpeed);

  delay(50);

  // for (int i=0; i<180; i++){
  //
  //  servo.write(i);
  //  delay(100);
  // }


}
