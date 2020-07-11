#include <Servo.h>
Servo servo;


void setup() {
  // put your setup code here, to run once:
  servo.attach(6);
  pinMode(A0, INPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  //
  int val = analogRead(A0);
  int motorSpeed = map(val, 0, 1024, 0, 180);
  Serial.println(motorSpeed);
  servo.write(motorSpeed);

  // for (int i=0; i<180; i++){
  //
  //  servo.write(i);
  //  delay(100);
  // }


}
