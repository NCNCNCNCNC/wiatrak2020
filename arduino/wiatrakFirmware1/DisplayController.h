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
    float scrollOffset = 0;
    float scrollSpeed = 0.1;
    int txtLen = 0;
    TextAlign txtAlign = ALIGN_LEFT;
    ScrollDir scrollDir = SCROLL_LEFT;

    unsigned long previousMillis = 0;
    unsigned long interval = 10;

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

    void updateImmediate() {

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
              scrollOffset += scrollSpeed;
            } else {
              scrollOffset = -(CHAR_COUNT - 1);
            }

          } else {

            if ( scrollOffset > -txtLen ) {
              scrollOffset -= scrollSpeed;
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

    void update() {

      unsigned long currentMillis = millis();

      if (currentMillis - previousMillis >= interval) {

        previousMillis = currentMillis;
        updateImmediate();
        
      }

    }

    void writeChar( int charIndex, char c ) {
      displays[ charIndex / CHAR_PER_DISPLAY ].writeDigitAscii( charIndex % CHAR_PER_DISPLAY, c );
    }

    void setTextAlign( TextAlign _txtAlign ) {
      txtAlign = _txtAlign;
    }

    void setScrollSpeed( float speed ) {
      scrollSpeed = speed;
    }

    void writeNumber( int num ) {
      char buff[16];
      sprintf(buff, "%d", num);
      writeText( buff );
    }

    void writeNumber( long num ) {
      char buff[16];
      sprintf(buff, "%ld", num);
      writeText( buff );
    }

    // TODO - draw dot - https://forums.adafruit.com/viewtopic.php?f=47&t=75183
    void writeNumber( float num, int decimal ) {
      char buff[16];
      String value = String(num, decimal); // TODO - replace String class usage...
      char *result = value.c_str();
      sprintf(buff, "%s", result); //formatBuff
      writeText( buff );
    }

    void writeText( char *text ) {

      txtLen = int(strlen(text));
      strcpy(buff, text);

      int offset = int(scrollOffset);

      for ( int i = 0; i < CHAR_COUNT; i++ ) {

        boolean setEmpty = ( offset > 0 ) ? ( i < offset || i - offset > txtLen - 1) : ( i - offset > txtLen - 1 );

        if ( setEmpty ) {
          writeChar(i, ' ' );
        } else {
          writeChar(i, text[i - offset] );
        }

      }

    }


};
