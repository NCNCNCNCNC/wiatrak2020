#include <Servo.h>
#include <TM1637Display.h>

#define FAN_PIN 6
Servo servo;
int angle = 10;
TM1637Display display (2, 3);

int values [] = {99, 983, 2827, 1515, 195, 51};
#define VALUES_COUNT 6
int currentIndex = 0;
int currentValue = 0;
int previousValue = 0;
int valueAnim = 0.0;
unsigned long previousMillis = 0;
unsigned long previousMillis2 = 0;
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
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;
    previousValue = currentValue;

    if (currentIndex < VALUES_COUNT - 1) {
      currentIndex ++;

    } else {
      currentIndex = 0;

    }
   Serial.println("aktualny index");
    Serial.println(currentIndex);
   Serial.println("_______");
    currentValue = values[currentIndex];

  }

  if (currentMillis - previousMillis2 >= 10) {
    
    previousMillis2 = currentMillis;

    int v = map( valueAnim, 0, 1000, previousValue, currentValue );

    if (valueAnim < 1000 ) {
      valueAnim += 100;
    } else {
      valueAnim = 0;

    }
    
    Serial.println( v );
    
//    int val = map(values[currentIndex], 50, 2827, 40, 250);
//    analogWrite(FAN_PIN, val);
//    Serial.println(val);
//    display.showNumberDec(values[currentIndex]);
//    
//    servo.write( currentIndex * 30 );

  }
}
