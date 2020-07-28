#include <Wire.h>
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"

#define DISPLAYS_NUM 2
#define CHAR_COUNT 8
#define CHAR_PER_DISPLAY 4

Adafruit_AlphaNum4 displays[2] = { Adafruit_AlphaNum4(), Adafruit_AlphaNum4() };

char buff[32];
int scrollOffset = 0;
int txtLen = 0;

unsigned long previousMillis = 0;
const long interval = 16;

class DisplayController {

  public:

    void setup() {

      for ( int i = 0; i < DISPLAYS_NUM; i ++ ) {
        displays[i].begin(0x70 + i);
        displays[i].setBrightness(15);
        displays[i].clear();
        displays[i].writeDisplay();
      }

      for ( int i = 0; i < 32; i++ ) {
        buff[i] = ' ';
      }

    }

    void update() {

      unsigned long currentMillis = millis();

      if (currentMillis - previousMillis >= interval) {

        previousMillis = currentMillis;


        //sprintf(buff, "%ld", long(value));
        //int strLen = int(strlen(buff));

        scrollOffset = CHAR_COUNT - txtLen; // RIGHT
        //scrollOffset = 0; // LEFT
        //scrollOffset = -CHAR_COUNT / 2 + txtLen / 2; // CENTER

        //        Serial.print( strLen );
        //        Serial.print( ", " );
        //        Serial.println( scrollOffset );
        //        Serial.println( int(value) );

        //setDisplayText( -scrollOffset, buff ); //diseaseNames[diseaseIndex]

        //  if ( scrollOffset < int(strlen(buff)) ) { // ????
        //    scrollOffset++;
        //  } else {
        //    scrollOffset = -(frameLength - 1);
        //  }

        //value += 100;

        for ( int i = 0; i < DISPLAYS_NUM; i++ ) {
          displays[i].writeDisplay();
        }

      }
    }

    void writeChar( int charIndex, char c ) {
      displays[ charIndex / CHAR_PER_DISPLAY ].writeDigitAscii( charIndex % CHAR_PER_DISPLAY, c );
    }

    void setDisplayText( char *text ) {

      txtLen = int(strlen(text));
      strcpy(buff, text);

      for ( int i = 0; i < CHAR_COUNT; i++ ) {

        boolean setEmpty = ( scrollOffset > 0 ) ? ( i < scrollOffset || i - scrollOffset > txtLen - 1) : ( i - scrollOffset > txtLen - 1 );

        if ( setEmpty ) {
          writeChar(i, ' ' );
        } else {
          writeChar(i, text[i - scrollOffset] );
        }

      }

    }


};
