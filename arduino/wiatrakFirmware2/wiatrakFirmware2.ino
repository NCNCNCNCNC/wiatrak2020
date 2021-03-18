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
#define INFO_COUNT 4

char *diseases[] = {"MEASLES", "TUBERCULOSIS", "COVID-19", "SWINE FLU", "MERS 2015"};
float deaths[DISEASE_COUNT][VALUES_COUNT] = {
  {535300, 454000, 164000, 122000, 89780, 140000},
  {1700000, 1670000, 1550000, 1450000, 1370000, 1310000},
  {40689, 142664, 167775, 155571, 275007, 412363},
  {99, 983, 1827, 1515, 195, 51},
  {6, 100, 277, 268, 30, 0}
};
float deathValBounds[DISEASE_COUNT][2] = {
  {89780, 535300},
  {1310000, 1700000},
  {142664, 412363},
  {51, 1827},
  {0, 277}
}
char *dates[DISEASE_COUNT][VALUES_COUNT] = {
  {"2000", "2004", "2008", "2012", "2016", "2018"},
  {"2000", "2003", "2006", "2009", "2012", "2015"},
  {"03/2020", "05/2020", "07/2020", "09/2020", "11/2020", "01/2021"},
  {"05/2009", "08/2009", "10/2009", "02/2010", "05/2010", "08/2010"},
  {"2012", "2013", "2014", "2015", "2016", "2017"}
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
unsigned long pauseDuration = 0; // ustawiane w setup na bazie pauz w info
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

    mainKeyframeBuffer[ i ] = { deaths[srcIndex][i], transitionDuration, Easing::easeInOutCubic, pauseDuration };
    
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
    angles[min(mainTimeline.getCurrentKeyIndex()+1, VALUES_COUNT-1)] 
  );

  return int( val ); 
  
}

int getCurrentMotorSpeed(){

  int motorSpeed = map(value, deathValBounds[currentDiseaseIndex][0], deathValBounds[currentDiseaseIndex][1], MIN_MOTOR_SPEED, MAX_MOTOR_SPEED);
  return motorSpeed;
  
}

void update() {
  
  mainTimeline.update();
  infoTimeline.update();
  
  displayController.update();

  servo.write( getCurrentServoPos() );
  motor.write( getCurrentMotorSpeed() );

  if( mainTimeline.isPaused() ){ // PAUZA

    if( !infoTimeline.isRunning() ){
      
      setInfoSequence();
      infoTimeline.play( infoKeyframeBuffer, 4 );
      Serial.println("start info seq");
      
    }else{

      Serial.print( "info key: " );
      Serial.println(  infoTimeline.getCurrentKeyIndex()  );
      
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
  
  
  


}



void loop() {
  
  update();
  
}
//EOF.
