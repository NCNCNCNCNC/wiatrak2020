#include <Servo.h>
#include <TM1637Display.h>

#define FAN_PIN 6
Servo servo;
int angle = 10;
TM1637Display display (2, 3);

int values [] = {99, 983, 2827, 1515, 195, 51};
#define VALUES_COUNT 6
int currentIndex = 0;
unsigned long previousMillis = 0;
const long interval = 1000;

void setup() {
  servo.attach(8);
  servo.write(angle);
  pinMode(FAN_PIN, OUTPUT);
  Serial.begin(9600);
  while (!Serial) {
    ;
  }
  display.setBrightness(105);

}


void loop()
{
  // no hejka !
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;

    if (currentIndex < VALUES_COUNT -1) {
      currentIndex ++;

    } else {
      currentIndex = 0;

    }
    Serial.println("aktualny index");
    Serial.println(currentIndex);
    Serial.println("_______");

    int val = map(values[currentIndex], 50, 2827, 40, 250);
    analogWrite(FAN_PIN, val);
    Serial.println(val);
    display.showNumberDec(values[currentIndex]);
    servo.write( currentIndex * 30);


  }


}


//
//  int value;
//  int val;
//map(value, 50, 3000, 20, 250);

//  analogWrite(Fan, 255);
//  delay(40);

//  value = 99;
//  val = map(value, 50, 2827, 40, 250);
//  analogWrite(Fan, val);
//  Serial.println(val);
//  delay(200);
//  display.showNumberDec(99);
//  delay(4000);
//
//  for (angle = 10; angle < 60; angle++)
//  {
//    servo.write(angle);
//    delay(15);
//
//  }
//
//
//  value = 983;
//  val = map(value, 50, 2827, 40, 250);
//  analogWrite(Fan, val);
//  Serial.println(val);
//  display.showNumberDec(983);
//  delay(4000);
//
//
//  for (angle = 60; angle < 120; angle++)
//  {
//    servo.write(angle);
//    delay(15);
//  }
//
//  value = 2827;
//  val = map(value, 50, 2827, 40, 250);
//  analogWrite(Fan, val);
//  Serial.println(val);
//  display.showNumberDec(2827);
//  delay(4000);
//
//  for (angle = 120; angle < 180; angle++)
//  {
//    servo.write(angle);
//    delay(15);
//  }
//
//  value = 1515;
//  val = map(value, 50, 2827, 40, 250);
//  analogWrite(Fan, val);
//  Serial.println(val);
//  display.showNumberDec(1515);
//  delay(4000);
//
//  for (angle = 180; angle > 120; angle--)
//  {
//    servo.write(angle);
//    delay(15);
//  }
//
//  value = 195;
//  val = map(value, 50, 2827, 40, 250);
//  analogWrite(Fan, val);
//  Serial.println(val);
//  display.showNumberDec(195);
//  delay(4000);
//
//  for (angle = 120; angle > 60; angle--)
//  {
//    servo.write(angle);
//    delay(15);
//  }
//
//  value = 51;
//  val = map(value, 50, 2827, 40, 250);
//  analogWrite(Fan, val);
//  Serial.println(val);
//  display.showNumberDec(51);
//  delay(4000);
//
//  for (angle = 60; angle > 10; angle--)
//  {
//    servo.write(angle);
//    delay(15);
//
//  }
//
//}
