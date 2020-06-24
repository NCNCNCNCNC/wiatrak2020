
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

// oddzielne zmienne dla 2 tweenow 
TweenDuino::Tween *valueTween;
TweenDuino::Tween *angleTween;


void setup() {
  Serial.begin(9600);
  valueTween = TweenDuino::Tween::to(value, 5000UL, values[0]);
  angleTween = TweenDuino::Tween::to(ang, 5000UL, angles[0]);
  display.setBrightness(105);
  pinMode(FAN_PIN, OUTPUT);
  servo.attach(8);
  servo.write(ang);
}


void loop() {

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
    delay(3025);
    valueTween = TweenDuino::Tween::to(value, 5000UL, values[currentIndex]);
    angleTween = TweenDuino::Tween::to(ang, 5000UL, angles[currentIndex]);
  }


  if (valueTween->isActive()) {
    display.showNumberDec(value);
    analogWrite(FAN_PIN, map(value, 50, 2827, 120, 250));
  }

  if (angleTween->isActive()) {
    servo.write(ang);
    //    servo.write( currentIndex * 30 );
    Serial.println(ang);
  }

}
