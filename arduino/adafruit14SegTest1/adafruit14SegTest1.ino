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

  float s = sin( millis() / 100 ) * 100;
  int b = map( s, -100, 100, 0, 15 );
  
  //https://arduinobasics.blogspot.com/2019/05/sprintf-function.html
  sprintf(buff,"%4d", int(value)); //

  for( int i = 0; i < 4; i++ ){
    alpha4.writeDigitAscii(i, buff[i]);
  }

  

  alpha4.setBrightness(b);
  alpha4.writeDisplay();

  value += 1;
  
  delay(5);
  
  
}
