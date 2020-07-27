#include <Wire.h>
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"

//Adafruit_AlphaNum4 alpha4_1 = Adafruit_AlphaNum4();
//Adafruit_AlphaNum4 alpha4_2 = Adafruit_AlphaNum4();
#define DISPLAYS_NUM 2
#define CHAR_COUNT 8
#define CHAR_PER_DISPLAY 4

Adafruit_AlphaNum4 displays[2] = { Adafruit_AlphaNum4(), Adafruit_AlphaNum4() };

float value = 0.0;
char buff[32];
char displayBuffer[8];
String txt = "TUBERCULOSIS";
int scrollOffset = 0;
int frameLength = 8;

void setup() {
  
  Serial.begin(9600);

  for( int i = 0; i < DISPLAYS_NUM; i ++ ){
    displays[i].begin(0x70+i);
    displays[i].setBrightness(12);
    displays[i].clear();
    displays[i].writeDisplay();
  }

  for( int i = 0; i < 32; i++ ){
    buff[i] = ' ';
  }

  for( int i = 0; i < txt.length(); i++ ){
    buff[i] = txt.charAt(i);
  }
  
}

void displaySet( int charIndex, char c ){
  displays[ charIndex / CHAR_PER_DISPLAY ].writeDigitAscii( charIndex % CHAR_PER_DISPLAY, c );
}

void setDisplayText( int offset, String text ){

  if( offset >= 0 ){
    
     for( int i = 0; i < 8; i++ ){
      
      if( i < offset ){
        displaySet(i, ' ' );
      }else{
        displaySet(i, text.charAt(i-offset) );
      }
      
     }
     
  }else{

    for( int i = 0; i < 8; i++ ){
      
      if( i+(offset*-1) > int(text.length())-1 ){
        displaySet(i, ' ' );
      }else{
        displaySet(i, text.charAt(i+(offset*-1)) );
      }
      
     }
    
  }
  
  for( int i = 0; i < DISPLAYS_NUM; i++ ){
    displays[i].writeDisplay();
  }
  
}


void loop() {

  float s = sin( millis() / 100 ) * 100;
  int b = map( s, -100, 100, 0, 15 );

  setDisplayText( -scrollOffset, txt );
  
  if( scrollOffset < int(txt.length()) ){ // ????
    scrollOffset++;
  }else{
    scrollOffset = -(frameLength-1);
  }
  
  delay(100);
  
  
}
