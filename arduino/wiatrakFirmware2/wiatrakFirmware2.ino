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

#define VALUES_COUNT 7
#define DISEASE_COUNT 5
#define INFO_COUNT 4

char *diseases[] = {"MEASLES", "TUBERCULOSIS", "COVID-19", "SWINE FLU", "MERS-COV"};
float deaths[DISEASE_COUNT][VALUES_COUNT] = {
  {1000000, 535300, 454000, 164000, 12000, 8978, 0},
  {2530000, 2977000, 3500000, 1600000, 1450000, 1800000, 0},
  {36375, 142261, 162935, 160842, 272129, 414975, 0},
  {99, 1369, 3056, 1515, 195, 51, 0},
  {6, 100, 277, 268, 47, 59, 0}
};

float deathValBounds[DISEASE_COUNT][2] = {
  {8978, 1000000},
  {1450000, 3500000},
  {36375, 414975},
  {51, 3056},
  {6, 277}
};
char *dates[DISEASE_COUNT][VALUES_COUNT] = {
  {"1996", "2000", "2004", "2008", "2012", "2016", "///"},
  {"1990", "1995", "2000", "2005", "2010", "2015", "///"},
  {"03/2020", "05/2020", "07/2020", "09/2020", "11/2020", "01/2021", "///"},
  {"05/2009", "08/2009", "11/2009", "02/2010", "05/2010", "08/2010", "///"},
  {"2012", "2013", "2014", "2015", "2016", "2017", "///"}
};
float angles [] = {0, 0, 36, 72, 108, 144, 180};
Keyframe mainKeyframeBuffer[VALUES_COUNT];
Timeline mainTimeline;
Keyframe infoKeyframeBuffer[INFO_COUNT];
Timeline infoTimeline;
int infoPauses[] = { 2000, 3000, 2000, 2000 };


int currentDiseaseIndex = 0;
int currentDeathIndex = 0;
float value = 0.0;
float ang = 0.0;

unsigned long transitionDuration = 3000;
unsigned long returnDuration = 8000;
unsigned long pauseDuration = 0; // ustawiane w setup na bazie pauz w info
unsigned long transitonTimer = 0;

int displayBlinkRate = 50;

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
  delay(8000); // zamkienic na 8000 docelowo
  digitalWrite( LED_BUILTIN, HIGH );

 
  pauseDuration = getPauseDuration();
  setupSequence(currentDiseaseIndex);
  startSequence();
  
}

unsigned long getPauseDuration(){
  unsigned long sum = 0;
  for( int i = 0; i < INFO_COUNT; i ++ ){
    sum += infoPauses[i];
  }
  return sum;
}

void setupSequence(int srcIndex){

  for( int i = 0; i < VALUES_COUNT; i ++ ){

    if( i < VALUES_COUNT-1 ){
      mainKeyframeBuffer[ i ] = { deaths[srcIndex][i], transitionDuration, Easing::easeInOutCubic, pauseDuration };
    }else{
      mainKeyframeBuffer[ i ] = { deaths[srcIndex][i], returnDuration, Easing::easeInOutCubic, 0 };
    }
    
  }
  
}

void setInfoSequence(){

  for( int i = 0; i < INFO_COUNT; i ++ ){

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
    angles[(mainTimeline.getCurrentKeyIndex()+1) % (VALUES_COUNT)]
  );

  return int( val ); 
  
}

int getCurrentMotorSpeed(){

  int motorSpeed = map(mainTimeline.getCurrentValue(), deathValBounds[currentDiseaseIndex][0], deathValBounds[currentDiseaseIndex][1], MIN_MOTOR_SPEED, MAX_MOTOR_SPEED);
  return motorSpeed;
  
}

void update() {
  
  mainTimeline.update();
  infoTimeline.update();
  
  displayController.update();

  servo.write( getCurrentServoPos() );

  int motorSpeed = ( mainTimeline.getCurrentKeyIndex() < VALUES_COUNT-1 ) ? getCurrentMotorSpeed() : MIN_MOTOR_SPEED;
  motor.write( getCurrentMotorSpeed() );

  
 // Serial.println( diseases[currentDiseaseIndex] );
  Serial.println( mainTimeline.getCurrentValue() );
  Serial.println( motorSpeed );
  //Serial.println( "----" );
  

  

  if( mainTimeline.isPaused() ){ // PAUZA

    if( !infoTimeline.isRunning() ){
      
      setInfoSequence(); // TODO: do usuniecia - raz w setup wystarczy ?
      infoTimeline.play( infoKeyframeBuffer, 4 );
      
    }else{
      
      switch ( infoTimeline.getCurrentKeyIndex() ){

        case 0: // WARTOSC
           displayController.setTextAlign( ALIGN_CENTER );
           displayController.writeNumber( long( mainTimeline.getCurrentValue() ) );
           displayController.setBlinkRate( displayBlinkRate );
        break;
        
        case 1: // NAZWA
           displayController.setTextAlign( ALIGN_FREE );
           displayController.writeText( diseases[currentDiseaseIndex] );
           displayController.setBlinkRate( 0 );
        break;

        case 2: // DATA
          displayController.setTextAlign( ALIGN_CENTER );
          displayController.writeText( dates[currentDiseaseIndex][mainTimeline.getCurrentKeyIndex()] );
          displayController.setBlinkRate( 0 );
        break;

         case 3: // WARTOSC
           displayController.setTextAlign( ALIGN_CENTER );
           displayController.writeNumber( long( mainTimeline.getCurrentValue() ) );
           displayController.setBlinkRate( displayBlinkRate );
        break;
        
      }
      
    }
    
  }else{ // PRZEJSCIE

    if( mainTimeline.getCurrentKeyIndex() < VALUES_COUNT-1 ){ // JESLI KOLEJNE DANE O ZGONACH
      displayController.setBlinkRate( 0 );
      displayController.setTextAlign( ALIGN_CENTER );
      displayController.writeNumber( long( mainTimeline.getCurrentValue() ) );
    }else{ // JESLI ANIMACJA POWROTU 
      displayController.setTextAlign( ALIGN_FREE );
      displayController.writeText( "*/*/*/*/" );
      displayController.setBlinkRate( 0 );
    }
    
    
  }

  if( mainTimeline.isFinished() ){
    
    currentDiseaseIndex = ((currentDiseaseIndex + 1) % DISEASE_COUNT);
    setupSequence( currentDiseaseIndex );
    mainTimeline.play( mainKeyframeBuffer, VALUES_COUNT, deathValBounds[currentDiseaseIndex][0] );
    

  }

}



void loop() {
  
  update();
  
}
//EOF.
