#include <Wire.h>
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"

#define DISPLAYS_NUM 2
#define CHAR_COUNT 8
#define CHAR_PER_DISPLAY 4

typedef enum {

  ALIGN_LEFT,
  ALIGN_RIGHT,
  ALIGN_CENTER,
  ALIGN_FREE

} TextAlign;

typedef enum {

  SCROLL_LEFT,
  SCROLL_RIGHT,

} ScrollDir;


class DisplayController {

  private:

    Adafruit_AlphaNum4 displays[2] = { Adafruit_AlphaNum4(), Adafruit_AlphaNum4() };

    char buff[32];
    int scrollOffset = 0;
    int txtLen = 0;
    TextAlign txtAlign = ALIGN_LEFT;
    ScrollDir scrollDir = SCROLL_LEFT;

    unsigned long previousMillis = 0;
    unsigned long interval = 200;

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

        switch ( txtAlign ) {

          case ALIGN_LEFT:
            scrollOffset = 0;
            break;

          case ALIGN_RIGHT:
            scrollOffset = CHAR_COUNT - txtLen;
            break;

          case ALIGN_CENTER:
            scrollOffset = CHAR_COUNT / 2 - txtLen / 2;
            break;

          case ALIGN_FREE:

            if ( scrollDir == SCROLL_RIGHT ) {

              if ( scrollOffset < txtLen ) {
                scrollOffset++;
              } else {
                scrollOffset = -(CHAR_COUNT - 1);
              }

            } else {

              if ( scrollOffset > -txtLen ) {
                scrollOffset--;
              } else {
                scrollOffset = CHAR_COUNT;
              }

            }

            break;

        }

        for ( int i = 0; i < DISPLAYS_NUM; i++ ) {
          displays[i].writeDisplay();
        }

      }



    }

    void writeChar( int charIndex, char c ) {
      displays[ charIndex / CHAR_PER_DISPLAY ].writeDigitAscii( charIndex % CHAR_PER_DISPLAY, c );
    }

    void setTextAlign( TextAlign _txtAlign ) {
      txtAlign = _txtAlign;
    }

    void setScrollSpeed( unsigned long speed ) {
      interval = speed;
    }

    void setDisplayText( char *text ) {

      txtLen = int(strlen(text));
      strcpy(buff, text);

      int offset = scrollOffset;

      for ( int i = 0; i < CHAR_COUNT; i++ ) {

        boolean setEmpty = ( offset > 0 ) ? ( i < offset || i - offset > txtLen - 1) : ( i - offset > txtLen - 1 );

        if ( setEmpty ) {
          writeChar(i, ' ' );
        } else {
          writeChar(i, text[i - scrollOffset] );
        }

      }

    }


};
