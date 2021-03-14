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
char *diseases[] = {"MEASLES", "TUBERCULOSIS", "COVID-19", "SWINE FLU", "MERS2015"};
float deaths[DISEASE_COUNT][VALUES_COUNT] = {
  {99, 983, 2827, 1515, 195, 51},
  {66, 1683, 3827, 3515, 695, 71},
  {129, 83, 5827, 4515, 1195, 31},
  {10, 120500, 827, 15, 1555, 10000},
  {999, 683, 1687, 155, 95, 99999}
};
float angles [] = {0, 36, 72, 108, 144, 180};
Keyframe keyframeBuffer[VALUES_COUNT];
Timeline mainTimeline;

int currentDiseaseIndex = 0;
int currentDeathIndex = 0;
float value = 0.0;
float ang = 0.0;

unsigned long transitionDuration = 5000;
unsigned long pauseDuration = 2000;
unsigned long transitonTimer = 0;
boolean isPaused = true;
float start_sec;

boolean sequenceStarted = false;

typedef enum {

  INIT,
  TRANSITION,
  STOP
  
} State;

State currentState = INIT;


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

    keyframeBuffer[ i ] = { deaths[srcIndex][i], transitionDuration, Easing::easeInOutCubic };
    
  }
  
}

void startSequence() {
  
  sequenceStarted = true;
  mainTimeline.play( keyframeBuffer, VALUES_COUNT );
  
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
  
  displayController.writeNumber( long( mainTimeline.getCurrentValue() ) );
  displayController.update();

  servo.write( getCurrentServoPos() );

  if( mainTimeline.isFinished() ){
    currentDiseaseIndex = ((currentDiseaseIndex + 1) % DISEASE_COUNT);
    setupSequence( currentDiseaseIndex );
    
    Serial.print("Disease index: ");
    Serial.println( currentDiseaseIndex );

    mainTimeline.play( keyframeBuffer, VALUES_COUNT );
    
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
