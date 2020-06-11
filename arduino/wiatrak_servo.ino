
#include <TweenDuino.h>
#include <TM1637Display.h>
#include <Servo.h>


int values [] = {99, 983, 2827, 1515, 195, 51};
int angles [] = {30, 60, 90, 120, 150, 180};
float ang = 0.0;
int currentIndex = 0;
float value = 20.0;
#define FAN_PIN 6
#define VALUES_COUNT 6
Servo servo;
//int angle = 10;
int k = 0;
TM1637Display display (2, 3);
TweenDuino::Tween *tween;


void setup() {
  Serial.begin(9600);
  tween = TweenDuino::Tween::to(value, 5000UL, values[0]);
  tween = TweenDuino::Tween::to(ang, 5000UL, angles[0]);
  display.setBrightness(105);
  pinMode(FAN_PIN, OUTPUT);
  servo.attach(8);
  servo.write(ang);
}


void loop() {

  tween->update(millis());

  if (tween->isComplete()) {

    Serial.println("DONE!");
    Serial.println(value);

    if (currentIndex < VALUES_COUNT - 1) {
      currentIndex ++;

    } else {
      currentIndex = 0;
    }
    delay(3025);
    tween = TweenDuino::Tween::to(value, 5000UL, values[currentIndex]);
    tween = TweenDuino::Tween::to(ang, 5000UL, angles[currentIndex]);
  }


  if (tween->isActive()) {
    display.showNumberDec(value);
    analogWrite(FAN_PIN, map(value, 50, 2827, 120, 250));
    servo.write(ang);
    //    servo.write( currentIndex * 30 );
    Serial.println(ang);
  }

}
