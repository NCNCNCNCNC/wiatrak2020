#include <Easing.h>
// 

typedef float (*KeyEasing)(float t, float b, float c, float d);

typedef struct _keyframe {
    float val;
    unsigned long dur;
    KeyEasing ease;
} Keyframe;


class Timeline {

  Keyframe* keyframes;
  int keyframeIndex = 0;
  int keyframeCount;

  float currentValue = 0.0;
  float initialValue = 0.0;
  float destinationValue = 0.0;
  float previousValue = 0.0;
  
  unsigned long timer;
  unsigned long duration;
  unsigned long keyDuration;

  bool running = false;
  bool finished = false;
  
  
  public:
  // ----------------------------------------------------
  
  void setup(){
    
  }

  void play( Keyframe* _key, int _keyCount ){

    play( _key, _keyCount, 0.0 ); 
    
  }

  void play( Keyframe* _key, int _keyCount, float _initialVal ){

    //Serial.println( "----- ANIM START -----" );
    
    keyframes = _key;
    keyframeCount = _keyCount;
    keyframeIndex = 0;

    initialValue = _initialVal;
    previousValue = initialValue;
    destinationValue = keyframes[ keyframeIndex ].val;
    keyDuration = keyframes[ keyframeIndex ].dur;
    
    timer = millis();
    running = true;
    finished = false;
    
  }

  void stop(){

    running = false;
    
  }

  void reset(){

    running = false;
    finished = false;
    keyframeIndex = 0;
    currentValue = 0;
    previousValue = initialValue;
    destinationValue = keyframes[ keyframeIndex ].val;
    keyDuration = keyframes[ keyframeIndex ].dur;

    //Serial.println( "----- ANIM STOPPED -----" );
    
  }

  void nextKeyframe(){

    currentValue = destinationValue; // HACK to avoid rounding erros;

    if( keyframeIndex < keyframeCount-1 ){

      keyframeIndex ++;
      
      previousValue = currentValue;
      destinationValue = keyframes[ keyframeIndex ].val;
      keyDuration = keyframes[ keyframeIndex ].dur;

      timer = millis();
      
      
    }else{

      if( running ){
         Serial.println( "----------- END --------------" );
         finished = true;
      }
      
      running = false;
      
    }
    
  }

  void update(){

    if( running ){
    
      double p = getCurrentKeyProgress();
  
      // DEBUG
//      Serial.print( keyframeIndex );
//      Serial.print( " " );
//      Serial.print( keyDuration );
//      Serial.print( " " );
//      Serial.print( previousValue );
//      Serial.print( " " );
//      Serial.print( destinationValue );
//      Serial.print( " " );
//      Serial.print( p );
//      Serial.print( " " );
//      Serial.println( currentValue );
      //
      
      if( millis() < timer + keyDuration ){
   
         currentValue = keyframes[ keyframeIndex ].ease( p, previousValue, destinationValue - previousValue, 1.0 );
         
      }else{
         
         nextKeyframe();
         
      }

      //

    }else{

      finished = false;
      
    }
    
  }

  float getCurrentRemappedValue( float startVal, float endVal  ){

    float val = keyframes[ keyframeIndex ].ease( getCurrentKeyProgress(), startVal, endVal - startVal, 1.0 );
    return val;
    
  }

  double getCurrentKeyProgress(){
    return min( double(millis() - timer) / double( keyDuration ), 1.0 );
  }

  int getCurrentKeyIndex(){
    return keyframeIndex;
  }

  float getDestinationValue(){
    return destinationValue;
  }

  float getCurrentValue() {
    return currentValue;
  }

  bool isRunning(){
    return running;
  }

  bool isFinished(){
    return finished;
  }

  
};
