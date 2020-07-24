#include <Wire.h>
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"

Adafruit_AlphaNum4 alpha4 = Adafruit_AlphaNum4();

float value = 0.0;
char buff[11];

void setup() {
  
  Serial.begin(9600);
  
  alpha4.begin(0x70);  // pass in the address
  alpha4.setBrightness(15);
  alpha4.clear();
  alpha4.writeDisplay();

  for( int i = 0; i < 11; i++ ){
    buff[i] = ' ';
  }
  
 

}

void loop() {
  // put your main code here, to run repeatedly:
  
  //dtostrf(value, 4, 1, buff);
  sprintf(buff,"%04d", int(value));
  
  alpha4.writeDigitAscii(0, buff[0]);
  alpha4.writeDigitAscii(1, buff[1]);
  alpha4.writeDigitAscii(2, buff[2]);
  alpha4.writeDigitAscii(3, buff[3]);

  alpha4.writeDisplay();

  value += 1;
  
  delay(10);
  
  
}
