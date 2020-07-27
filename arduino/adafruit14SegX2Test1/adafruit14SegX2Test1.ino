#include <Wire.h>
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"

#define DISPLAYS_NUM 2
#define CHAR_COUNT 8
#define CHAR_PER_DISPLAY 4

Adafruit_AlphaNum4 displays[2] = { Adafruit_AlphaNum4(), Adafruit_AlphaNum4() };

float value = 0.0;
char buff[32];

const char *diseaseNames[] = {
  "TUBERCULOSIS", "MEASLES", "COVID-19", "SWINE FLU", "MERS 2015"
};
int diseaseIndex = 1;


int scrollOffset = 0;
int frameLength = 8;

void setup() {

  Serial.begin(9600);

  for ( int i = 0; i < DISPLAYS_NUM; i ++ ) {
    displays[i].begin(0x70 + i);
    displays[i].setBrightness(12);
    displays[i].clear();
    displays[i].writeDisplay();
  }

  for ( int i = 0; i < 32; i++ ) {
    buff[i] = ' ';
  }

}

void displaySet( int charIndex, char c ) {
  displays[ charIndex / CHAR_PER_DISPLAY ].writeDigitAscii( charIndex % CHAR_PER_DISPLAY, c );
}

void setDisplayText( int offset, char *text ) {

  int txtLen = int(strlen(text));
  
  for ( int i = 0; i < CHAR_COUNT; i++ ) {
    
    boolean setEmpty = ( offset > 0 ) ? ( i < offset || i - offset > txtLen - 1) : ( i - offset > txtLen - 1 );

    if ( setEmpty ) {
      displaySet(i, ' ' );
    } else {
      displaySet(i, text[i - offset] );
    }

   }

  for ( int i = 0; i < DISPLAYS_NUM; i++ ) {
    displays[i].writeDisplay();
  }

}


void loop() {

  float s = sin( millis() / 100 ) * 100;
  int b = map( s, -100, 100, 0, 15 );

  sprintf(buff,"%d", int(value));

  int strLen = int(strlen(buff));

  //scrollOffset = CHAR_COUNT - strLen; // LEFT
  scrollOffset = -CHAR_COUNT/2 + strLen/2; // CENTER
  Serial.print( strLen );
  Serial.print( ", " );
  Serial.println( scrollOffset );
  Serial.println( int(value) );

  setDisplayText( -scrollOffset, buff ); //diseaseNames[diseaseIndex]



//  if ( scrollOffset < int(strlen(buff)) ) { // ????
//    scrollOffset++;
//  } else {
//    scrollOffset = -(frameLength - 1);
//  }

  value += 100;

  delay(5);


}
