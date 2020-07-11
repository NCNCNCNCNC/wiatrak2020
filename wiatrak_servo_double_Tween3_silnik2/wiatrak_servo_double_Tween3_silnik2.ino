#include <TweenDuino.h>
#include <TM1637Display.h>
#include <Servo.h>
int values [] = {99, 99, 983, 983, 2827, 2827, 1515, 1515, 195, 195, 51, 51};
int angles [] = {30, 30, 60, 60, 90, 90, 120, 120, 150, 150, 180, 180};
float ang = 0.0;
int currentIndex = 0;
float value = 20.0;
#define FAN_PIN 6
#define VALUES_COUNT 12
Servo servo;
//int angle = 10;
int k = 0;
TM1637Display display (2, 3);
unsigned long transitionDuration = 8000;
unsigned long pouseDuration = 3000;


// oddzielne zmienne dla 2 tweenow
TweenDuino::Tween *valueTween;
TweenDuino::Tween *angleTween;


void setup() {
  Serial.begin(9600);
  valueTween = TweenDuino::Tween::to(value, transitionDuration, values[0]);
  angleTween = TweenDuino::Tween::to(ang, transitionDuration, angles[0]);
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
    display.showNumberDec(value);
    analogWrite(FAN_PIN, map(value, 50, 2827, 120, 250));
  }

  if (angleTween->isActive()) {
    servo.write(ang);
    //    servo.write( currentIndex * 30 );
    Serial.println(ang);
  }
}
