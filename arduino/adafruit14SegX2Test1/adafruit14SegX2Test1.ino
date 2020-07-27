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

void setDisplayBuffer( int offset, String text ){

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
  

  
}


void loop() {

  float s = sin( millis() / 100 ) * 100;
  int b = map( s, -100, 100, 0, 15 );

//  for( int i = 0; i < 8; i++ ){
//    displaySet( i, buff[i] );
//  }

  setDisplayBuffer( -2, txt );
  
  for( int i = 0; i < DISPLAYS_NUM; i++ ){
    displays[i].writeDisplay();
  }
  

//  int txtIndex = 0;
//  for( int di = 0; di < DISPLAYS_NUM; di ++ ){
//
//    for( int ci = 0; ci < CHAR_PER_DISPLAY; ci ++ ){
//
////      Serial.print( di );
////      Serial.print( ", " );
////      Serial.print( ci );
////      Serial.print( ", " );
//
//      if( scrollOffset < 0 ){
//
//        if( ci - scrollOffset < 4 ){
//          displays[di].writeDigitAscii(ci - scrollOffset, buff[txtIndex]); // -- = +
//        }
//        
////        
////        Serial.print( ci - scrollOffset );
////        Serial.print( ", " );
////        Serial.print( buff[txtIndex] );
//      }else{
//        displays[di].writeDigitAscii(ci, buff[txtIndex + scrollOffset]);
//        
////        Serial.print( txtIndex + scrollOffset );
////        Serial.print( ", " );
////        Serial.print( buff[txtIndex + scrollOffset] );
//      }
//
//      //Serial.println( "---" );
//      txtIndex++;
//      //displays[di].writeDigitAscii( ci, txt.charAt( txtIndex++ ) );
//        
//    }
//
//    displays[di].writeDisplay();
//  
//  }
  
  //https://arduinobasics.blogspot.com/2019/05/sprintf-function.html
  //sprintf(buff,"%4d", int(value));//

//  for( int i = 0; i < 4; i++ ){
//
//    if( scrollOffset < 0 ){
//      alpha4.writeDigitAscii(i - scrollOffset, buff[i]); // -- = +
//    }else{
//      alpha4.writeDigitAscii(i, buff[i + scrollOffset]);
//    }
//    
//  }
//
//  //alpha4.setBrightness(b);
//  alpha4.writeDisplay();
//
//  value += 1;
//
  if( scrollOffset < int(txt.length()) ){ // ????
    scrollOffset++;
  }else{
    scrollOffset = -(frameLength-1);
  }


  
  
  delay(300);
  
  
}
