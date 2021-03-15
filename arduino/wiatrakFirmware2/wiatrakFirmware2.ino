// WIATRAK FRIRMWARE v2

#include <Servo.h>
#include "Timeline.h"
#include "DisplayController.h"

DisplayController displayController;

Servo motor;
Servo servo;

#define MOTOR_PIN 6
#define SERVO_PIN 3
#define MIN_MOTOR_SPEED 45
#define MAX_MOTOR_SPEED 140
#define MOTOR_INIT_VAL 30
#define MIN_SERVO_MS 500
#define MAX_SERVO_MS 2500

#define VALUES_COUNT 6
#define DISEASE_COUNT 5
char *diseases[] = {"MEASLES", "TUBERCULOSIS", "COVID-19", "SWINE FLU", "MERS 2015"};
float deaths[DISEASE_COUNT][VALUES_COUNT] = {
  {99, 983, 2827, 1515, 195, 51},
  {66, 1683, 3827, 3515, 695, 71},
  {129, 83, 5827, 4515, 1195, 31},
  {10, 120500, 827, 15, 1555, 10000},
  {999, 683, 1687, 155, 95, 99999}
};
char *dates[DISEASE_COUNT][VALUES_COUNT] = {
  {"01/2001", "02/2001", "03/2001", "04/2001", "05/2001"},
  {"01/2002", "02/2002", "03/2002", "04/2002", "05/2002"},
  {"01/2003", "02/2003", "03/2003", "04/2003", "05/2003"},
  {"01/2004", "02/2004", "03/2004", "04/2004", "05/2004"},
  {"01/2005", "02/2005", "03/2005", "04/2005", "05/2005"},
};
float angles [] = {0, 36, 72, 108, 144, 180};
Keyframe mainKeyframeBuffer[VALUES_COUNT];
Timeline mainTimeline;
Keyframe infoKeyframeBuffer[VALUES_COUNT];
Timeline infoTimeline;
int infoPauses[] = { 1000, 3000, 2000, 1000 };


int currentDiseaseIndex = 0;
int currentDeathIndex = 0;
float value = 0.0;
float ang = 0.0;

unsigned long transitionDuration = 5000;
unsigned long pauseDuration = 8500;
unsigned long transitonTimer = 0;
boolean isPaused = true;
float start_sec;

boolean sequenceStarted = false;

void setup() {

  //pinMode(A0, INPUT);
  pinMode(MOTOR_PIN, OUTPUT);
  pinMode(SERVO_PIN, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  //digitalWrite(MOTOR_PIN, LOW);
  //delay(10);
  
  motor.attach(MOTOR_PIN);
  motor.write(MOTOR_INIT_VAL); // nie moze byc zero
  servo.attach(SERVO_PIN, MIN_SERVO_MS, MAX_SERVO_MS);
  servo.write(0);

  Serial.begin(9600);

  displayController.setup();
  displayController.setTextAlign( ALIGN_CENTER );
  displayController.writeText( "HELLO123" );
  displayController.updateImmediate();
  
  digitalWrite( LED_BUILTIN, LOW );
  delay(1000);
  digitalWrite( LED_BUILTIN, HIGH );

  setupSequence(currentDiseaseIndex);
  startSequence();
  
}

void setupSequence(int srcIndex){

  for( int i = 0; i < VALUES_COUNT; i ++ ){

    mainKeyframeBuffer[ i ] = { deaths[srcIndex][i], transitionDuration, Easing::easeInOutCubic, pauseDuration };
    
  }
  
}

void setInfoSequence(){

  for( int i = 0; i < 4; i ++ ){

    infoKeyframeBuffer[ i ] = { 100, infoPauses[i], Easing::easeInOutCubic, 0 };
    
  }
  
}

void startSequence() {
  
  sequenceStarted = true;
  mainTimeline.play( mainKeyframeBuffer, VALUES_COUNT );
  
}

int getCurrentServoPos(){
  
  float val = mainTimeline.getCurrentRemappedValue( 
    angles[mainTimeline.getCurrentKeyIndex()], 
    angles[min(mainTimeline.getCurrentKeyIndex()+1, VALUES_COUNT-1)] 
  );

  return int( val ); 
  
}

void update() {
  
  mainTimeline.update();
  displayController.update();

  servo.write( getCurrentServoPos() );

  if( mainTimeline.isPaused() ){ // PAUZA

    if( !infoTimeline.isRunning() ){
      
      setInfoSequence();
      infoTimeline.play( infoKeyframeBuffer, 4 );
      Serial.println("start info seq");
      
    }else{
      
      switch ( infoTimeline.getCurrentKeyIndex() ){

        case 0: // WARTOSC
           displayController.setTextAlign( ALIGN_CENTER );
           displayController.writeNumber( long( mainTimeline.getCurrentValue() ) );
        break;
        
        case 1: // NAZWA
           displayController.setTextAlign( ALIGN_FREE );
           displayController.writeText( diseases[currentDiseaseIndex] );
        break;

        case 2: // DATA
          displayController.setTextAlign( ALIGN_CENTER );
          displayController.writeText( dates[currentDiseaseIndex][mainTimeline.getCurrentKeyIndex()] );
        break;

         case 3: // WARTOSC
           displayController.setTextAlign( ALIGN_CENTER );
           displayController.writeNumber( long( mainTimeline.getCurrentValue() ) );
        break;
        
      }
      
    }

    infoTimeline.update();
    
  }else{ // PRZEJSCIE
    
    displayController.setTextAlign( ALIGN_CENTER );
    displayController.writeNumber( long( mainTimeline.getCurrentValue() ) );
    
  }

  if( mainTimeline.isFinished() ){
    
    currentDiseaseIndex = ((currentDiseaseIndex + 1) % DISEASE_COUNT);
    setupSequence( currentDiseaseIndex );
    mainTimeline.play( mainKeyframeBuffer, VALUES_COUNT );
    
    Serial.print("Disease index: ");
    Serial.println( currentDiseaseIndex );

  }

  //Serial.print( mainTimeline.getCurrentValue());
  //Serial.print( ", " );
  //Serial.println( getCurrentServoPos() );
  
  //int motorSpeed = map(value, 51, 2827, MIN_MOTOR_SPEED, MAX_MOTOR_SPEED);
  //motor.write(motorSpeed);
  //servo.write(int(ang));


}



void loop() {
  
  update();
  
}
//EOF.
